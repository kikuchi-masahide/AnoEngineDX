import React,{FC} from 'react';

type ShowDirectoryComponentPropType = {
    //変換するディレクトリのパス
    srcDirectory:string,
    //変換したものを保存するディレクトリのパス
    dstDirectory:string,
    //変換するディレクトリを選ぶ際のonclick
    srcOnClick:() => void,
    //保存するディレクトリを選ぶ際のonclick
    dstOnClick:() => void
};

const ShowDirectoryComponent:FC<ShowDirectoryComponentPropType> = (props) => {
    return(
        <>
            <div className='mt-3 mb-3'>
                <label className='form-label'>変換ディレクトリの絶対パス</label>
                <div className='input-group'>
                    <button className='btn btn-primary' onClick={props.srcOnClick}>開く</button>
                    <input type='text' className='form-control' defaultValue={props.srcDirectory} readOnly/>
                </div>
            </div>
            <div className='mt-3 mb-3'>
                <label className='form-label'>保存ディレクトリの絶対パス</label>
                <div className='input-group'>
                    <button className='btn btn-primary' onClick={props.dstOnClick}>開く</button>
                    <input type='text' className='form-control' defaultValue={props.dstDirectory} readOnly/>
                </div>
            </div>
        </>
    );
};

export default ShowDirectoryComponent;