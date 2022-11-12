import {app,BrowserWindow,ipcMain,dialog,session,MessagePortMain,Menu} from 'electron';
import path from 'path';
import fs from 'fs';
import cp from 'child_process';
import ICookie from './@types/cookiedecl';
import {searchDevtools} from 'electron-search-devtools';

const isDev = process.env.NODE_ENV === 'development';
let mainWindow:Electron.CrossProcessExports.BrowserWindow;
//現在開いているプロファイルのフルパス
let currentProfileFullpath = '';
let cookie:ICookie = {
    pathbefore:'C://',
    pathafter:'C://',
    exts:[],
    excludeFiles:[]
};

if (isDev) {
    const execPath =
      process.platform === 'win32'
        ? '../node_modules/electron/dist/electron.exe'
        : '../node_modules/.bin/electron';
  
    require('electron-reload')(__dirname, {
      electron: path.resolve(__dirname, execPath),
    });
}

const createWindow = () => {
    mainWindow = new BrowserWindow({
      webPreferences: {
        preload: path.join(__dirname, 'preload.js'),
      },
    });

    const menu = Menu.buildFromTemplate([
        {
            label:"プロファイル読み込み",
            click:LoadProfile
        },
        {
            label:'プロファイル保存',
            click:ShowSaveProfileDialog
        }
    ]);
  
    ipcMain.on('update-title', (_e, arg) => {
      mainWindow.setTitle(`Electron React TypeScript: ${arg}`);
    });
  
    if (isDev) {
      searchDevtools('REACT')
        .then((devtools) => {
          session.defaultSession.loadExtension(devtools, {
            allowFileAccess: true,
          });
        })
        .catch((err) => console.log(err));
  
      mainWindow.webContents.openDevTools({ mode: 'detach' });
    }
  
    mainWindow.loadFile('dist/index.html');
    Menu.setApplicationMenu(menu);
};
  
const sendCookie = () => {
    const cookie = JSON.parse(fs.readFileSync('./src/cookie.json','utf8')) as ICookie;
    return cookie;
};

//'openFileDialog'がinvokeされた際呼び出す関数
const openFileDialogCallback = () => {
    return dialog.showOpenDialogSync(mainWindow,{
        properties: ['openDirectory','showHiddenFiles']
    });
};

//'startConvert'がinvokeされた際呼び出す関数
//TODO:今変換先フォルダに含まれるファイルを前削除しているので、それを改善する
const startConvertCallback = (cookie:ICookie,port:MessagePortMain) => {
    //変換先の削除
    fs.rmSync(cookie.pathafter,{recursive:true,force:true});
    fs.mkdirSync(cookie.pathafter);
    const regexps = cookie.excludeFiles.map((exc) => new RegExp(exc));
    port.start();
    //受け取ったpathbeforeからの相対パスrelpathに含まれるファイルを再帰的に変換
    const recur = (relpath:string) => {
        const dirfilenames = fs.readdirSync(path.join(cookie.pathbefore,relpath));
        dirfilenames.forEach(element => {
            const fullpathbefore = path.join(cookie.pathbefore,relpath,element);
            const stats = fs.statSync(fullpathbefore);
            if(stats.isFile()){
                //ファイルなので変換する
                //除外対象でないならばfalse
                let regexpcheck:boolean = false;
                for(let i = 0;i < regexps.length;i++){
                    const regexp = regexps[i];
                    if(regexp.test(path.join(relpath,element))){
                        regexpcheck = true;
                        break;
                    }
                }
                if(regexpcheck){
                    port.postMessage(`skipped:   ${path.join(relpath,element)}`);
                    return;
                }
                //"."で区切って拡張子を得る
                const sp = element.split('.');
                const extbefore = sp[sp.length-1];
                let fullpathafter = '';
                //無拡張子の場合
                if(sp.length === 1){
                    fullpathafter = path.join(cookie.pathafter,relpath,element);
                }else{
                    //拡張子の変換
                    let extafter = '';
                    cookie.exts.forEach(([b,a]:[string,string]) => {
                        if(b === extbefore){
                            extafter = a;
                        }
                    });
                    //見つからなかった
                    if(extafter === ''){
                        extafter = extbefore;
                    }
                    fullpathafter = path.join(cookie.pathafter,relpath,element.substring(0,element.length-extbefore.length)+extafter);
                }
                cp.execSync(`Crypto.exe ${fullpathbefore} ${fullpathafter}`);
                //renderに送信
                port.postMessage(`converted:${path.join(relpath,element)} -> ${fullpathafter}`);
            }else if(stats.isDirectory()){
                //フォルダなので再帰
                const fullpathafter = path.join(cookie.pathafter,relpath,element);
                fs.mkdirSync(fullpathafter);
                recur(path.join(relpath,element));
            }
        });
    };
    recur('');
    //変換終了したのでportを閉じる
    port.close();
};

//メニューから「プロファイル読み込み」を選択した際の挙動
function LoadProfile(){
    const ret = dialog.showOpenDialogSync(mainWindow,{
        properties: ['openFile','showHiddenFiles'],
        filters:[
            {name:'profile',extensions:['crpprf']}
        ]
    });
    if(typeof ret !== 'undefined'){
        const text = fs.readFileSync(ret[0],'utf-8');
        const json:ICookie = JSON.parse(text);
        mainWindow.webContents.send('setCookie',json);
    }
}

//メニューから「プロファイル保存」を選択した際の挙動
function ShowSaveProfileDialog(){
    const ret = dialog.showSaveDialogSync({
        defaultPath:currentProfileFullpath,
        filters:[
            {name:"profile",extensions:["crpprf"]}
        ]
    });
    if(typeof ret !== 'undefined'){
        //ファイルが存在しない場合とりあえず作成
        /*if(!fs.existsSync(ret)){
            fs.mkdirSync(ret);
        }*/
        fs.writeFileSync(ret,JSON.stringify(cookie));
    }
}

app.whenReady().then(() => {
    //Renderer側は、Cookieを受け取る準備ができたら、eapiから'cookie:ready'をinvokeする
    ipcMain.handle('cookie:ready',sendCookie);
    //Renderer側は、ファイルを開くダイアログが開きたくなったら、'openFileDialog'をinvokeする
    ipcMain.handle('openFileDialog',openFileDialogCallback);
    ipcMain.on('startConvert',(e,msg:ICookie)=>{
        const [port] = e.ports;
        startConvertCallback(msg,port);
    });
    ipcMain.on('informCookieChange',(_,msg:ICookie) => {
        cookie = msg;
    });
    createWindow();
    app.on('activate',function(){
        if(BrowserWindow.getAllWindows().length === 0){
            createWindow();
        }
    });
});

app.on('window-all-closed',function(){
    if(process.platform !== 'darwin'){
        app.quit();
    }
});