import React,{ FC, FormEventHandler, MouseEventHandler, useState } from 'react';

//AddExtComponentに渡されるpropsの型定義
type AddExcludeFileComponentPropsType = {
    //extsにこの拡張子の組を登録するための関数
    addFile:(file:string) => void
};

const AddExcludeFileComponent:FC<AddExcludeFileComponentPropsType> = (props) => {
    const [file,fileSetter] = useState<string>('');
    //fileが適切(追加しても問題ない)か
    const isFileValid = () => {
        return true;
        //空白などを含まない適切なパターンか?
        const regex = new RegExp('^[a-zA-Z0-9./_ ]+$');
        return(regex.test(file));
    };
    //変換前拡張子が変更された際のcallback
    const addfileOnInputCallback:FormEventHandler<HTMLInputElement> = (event) => {
        const value = event.currentTarget.value;
        fileSetter(value);
    };
    const addFileButtonOnClickCallback:MouseEventHandler<HTMLButtonElement> = (_) => {
        props.addFile(file);
        fileSetter('');
    };
    return(
        <>
            <label className='form-label'>除外ファイルの追加</label>
            <div className='container'>
                <div className='input-group mb-1'>
                    <input type='text' className='form-control' id='addfile' onInput={addfileOnInputCallback} value={file}/>
                </div>
               {
                    isFileValid()?
                        <button type="button" className='btn btn-primary' onClick={addFileButtonOnClickCallback}>追加</button>:
                        <button type="button" className='btn btn-primary' disabled>追加</button>
                }
            </div>
        </>
    );
};

export default AddExcludeFileComponent;