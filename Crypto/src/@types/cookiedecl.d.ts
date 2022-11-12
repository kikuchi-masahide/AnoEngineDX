//cookie.jsonの型情報
type ICookie = {
    //変換する前のディレクトリの絶対パス
    pathbefore:string;
    //変換したものを保存するディレクトリの絶対パス
    pathafter:string;
    //拡張子の変換の仕方
    //変換前拡張子、変換後拡張子のタプルとする(png,oftmなど)
    exts:Array<[string,string]>;
    //除外するファイルのパターン
    excludeFiles:Array<string>;
};

export default ICookie;
