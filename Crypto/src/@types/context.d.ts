import ICookie from './cookiedecl';

export interface IElectronAPI {
  //ファイルを開くダイアログを開く際実行する関数 引数はダイアログから受け取ったファイルパスを渡すセッター関数
  invokeOpenFileDialog:(pathsetter:(path:string) => void) => void;
  //ICookie情報から変換スタートメッセージ('startConvert')をinvokeする。
  //stringのメッセージが来た時のコールバックと、close時のコールバックを引数に渡す
  invokeConvertStart:(cookie:ICookie,msgcallback:(msg:string) => void,clscallback:() => void) => void;
  //メインスレッドからのICookie情報を受け取るコールバックを指定する関数
  setCookieReciever:(reciever:(cookie:ICookie) => void) => void;
  //レンダラーでのcookie情報の変更をメインに通知する
  informCookieChange:(cookie:ICookie) => void;
}

declare global {
  interface Window {
    myAPI: IElectronAPI;
  }
}
