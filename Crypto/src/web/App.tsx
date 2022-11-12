import React,{ useEffect, useState } from 'react';
import ShowExtComponent from './ShowExtComponent';
import ShowDirectoryComponent from './ShowDirectoryComponent';
import ICookie from '../@types/cookiedecl';
import AddExtComponent from './AddExtComponent';
import ShowExcludeFileComponent from './ShowExcludeFileComponent';
import AddExcludeFileComponent from './AddExcludeFileComponent';
import {useEffectOnce,useList,useLatest} from 'react-use';

//import Demo from './Demo';

const {myAPI} = window;

function App() {
  const [pathbefore,setPathbefore] = useState<string>("");
  const [pathafter,setPathafter] = useState<string>("");
  const [exts,extsSetter] = useList<[string,string]>([]);
  const [excludeFiles,excludeFilesSetter] = useList<string>([]);
  const [convertLog,convertLogSetter] = useList<string>([]);
  //変換ボタンを押せるならばtrue
  const [buttonAvailability,setButtonAvailability] = useState<boolean>(true);
  
  //受け取ったCookie.jsonの情報から各種ステートをセット
  const cookieReciever = (cookie:ICookie) => {
    console.log('loadCookie');
    setPathbefore(cookie.pathbefore);
    setPathafter(cookie.pathafter);
    extsSetter.set(cookie.exts);
    excludeFilesSetter.set(cookie.excludeFiles);
  };
  //extsにbeforeが与拡張子である組が存在するか検索する関数(AddExtComponentに渡す)
  const searchExtBeforeFromExts = (ext:string) => {
    let flag = false;
    exts.forEach((value:[string,string]) => {
      if(value[0] === ext){
        flag = true;
      }
    });
    return flag;
  };
  useEffectOnce(() => {
    myAPI.setCookieReciever(cookieReciever);
    cookieReciever({
      pathbefore:"C:\\",
      pathafter:"C:\\",
      exts:[],
      excludeFiles:[],
    })
    return () => {
      console.log('unmount');
    };
  });
  //cookie情報が変更された場合の処理
  useEffect(() => {
    myAPI.informCookieChange({
      pathbefore,
      pathafter,
      exts,
      excludeFiles
    });
  },[pathbefore,pathafter,exts,excludeFiles]);
  const buttononclick = () => {
    setButtonAvailability(false);
    convertLogSetter.set(["start convert\n========================================"]);
    const cookie:ICookie = {
      pathbefore:pathbefore,
      pathafter:pathafter,
      exts:exts,
      excludeFiles:excludeFiles
    };
    myAPI.invokeConvertStart(cookie,(msg:string) => {
      convertLogSetter.push(msg);
    },() => {
      convertLogSetter.push('========================================\nend convert');
      setButtonAvailability(true);
    });
  };
    return (
      <div className='container'>
        <ShowDirectoryComponent srcDirectory={pathbefore} dstDirectory={pathafter}
          srcOnClick={() => myAPI.invokeOpenFileDialog(setPathbefore)} dstOnClick={() => myAPI.invokeOpenFileDialog(setPathafter)}/>
        <ShowExtComponent exts={exts} onClick={extsSetter.removeAt}/>
        <AddExtComponent searchExtBeforeFromExts={searchExtBeforeFromExts} addExtToExts={extsSetter.push}/>
        <ShowExcludeFileComponent files={excludeFiles} onClick={excludeFilesSetter.removeAt}/>
        <AddExcludeFileComponent addFile={excludeFilesSetter.push}/>
        <div className='container'>
          {
            buttonAvailability?
            <button type="button" className='btn btn-primary btn-lg' onClick={buttononclick}>変換</button>:
            <button type="button" className='btn btn-primary btn-lg' onClick={buttononclick} disabled>変換</button>
          }
        </div>
        <textarea className='form-control' rows={16} value={convertLog.join('\n')} readOnly></textarea>
      </div>
    );
}

export default App;
