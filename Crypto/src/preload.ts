import ICookie from './@types/cookiedecl';
const {contextBridge,ipcRenderer} = require('electron');

contextBridge.exposeInMainWorld('myAPI',{
    invokeOpenFileDialog:(callback:(path:string) => void) => {
        //ファイルを開くダイアログの結果を、string[]またはundefinedで受け取る
        const pr = ipcRenderer.invoke('openFileDialog');
        pr.then((e) => {
            if(e !== undefined){
                callback(e[0]);
            }
        });
    },
    invokeConvertStart:(cookie:ICookie,msgcallback:(msg:string) => void,clscallback:() => void) => {
        //rendererで用いるport1とmainに送るport2
        const {port1,port2} = new MessageChannel();
        port1.addEventListener('message',(event) => {
            if(typeof event.data === 'string'){
                msgcallback(event.data);
            }
        });
        port1.addEventListener('close',() => {
            clscallback();
        });
        ipcRenderer.postMessage('startConvert',cookie,[port2]);
        port1.start();
    },
    setCookieReciever:(reciever:(cookie:ICookie) => void) => {
        //"setCookie"を受け取ったときのデータ受け取り先にrecieverを指定
        ipcRenderer.on("setCookie",(_,cookie:ICookie) => {
            reciever(cookie);
        });
    },
    informCookieChange:(cookie:ICookie) => {
        ipcRenderer.send('informCookieChange',cookie);
    }
});