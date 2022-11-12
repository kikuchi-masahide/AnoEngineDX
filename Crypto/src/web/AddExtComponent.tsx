import React,{ FC, FormEventHandler, MouseEventHandler, useState } from 'react';

//AddExtComponentに渡されるpropsの型定義
type AddExtComponentPropsType = {
    //extsにこの拡張子の組を登録するための関数
    addExtToExts:(ext:[string,string]) => void,
    //拡張子extが既に変換前拡張子としてextsに登録されているか検索する関数 登録済ならばtrue
    searchExtBeforeFromExts:(ext:string) => boolean
};

const AddExtComponent:FC<AddExtComponentPropsType> = (props) => {
    //現在入力されている拡張子
    const [extbefore,setExtBefore] = useState<string>('');
    const [extafter,setExtAfter] = useState<string>('');
    //[extbefore,extafter]が適切(=extsに追加しても問題ない)か
    const isExtBeforeAfterValid = () => {
        //空白などを含まない適切なパターンか?
        const regex = new RegExp('^[a-zA-Z0-9]+$');
        if(!regex.test(extbefore) || !regex.test(extafter)){
            return false;
        }
        //変換前拡張子がextsに登録済みなら不適切
        if(props.searchExtBeforeFromExts(extbefore)){
            return false;
        }
        return true;
    };
    //変換前拡張子が変更された際のcallback
    const addextbeforeOnInputCallback:FormEventHandler<HTMLInputElement> = (event) => {
        const value = event.currentTarget.value;
        setExtBefore(value);
    };
    //変換後拡張子が変更された際のcallback
    const addextafterOnInputCallback:FormEventHandler<HTMLInputElement> = (event) => {
        const value = event.currentTarget.value;
        setExtAfter(value);
    };
    //追加ボタンが押されたときのcallback
    const addExtButtonOnClickCallback:MouseEventHandler<HTMLButtonElement> = (_) => {
        props.addExtToExts([extbefore,extafter]);
        setExtBefore('');
        setExtAfter('');
    };
    return(
        <>
            <label className='form-label'>拡張子変換の追加</label>
            <div className='container'>
                <div className='input-group mb-1'>
                    <input type='text' className='form-control' id='addextbefore' onInput={addextbeforeOnInputCallback} value={extbefore}/>
                    <span className='input-group-text'> to </span>
                    <input type='text' className='form-control' id='addextafter' onInput={addextafterOnInputCallback} value={extafter}/>
                </div>
                {
                    isExtBeforeAfterValid()?
                        <button type="button" className='btn btn-primary' onClick={addExtButtonOnClickCallback}>追加</button>:
                        <button type="button" className='btn btn-primary' disabled>追加</button>
                }
            </div>
        </>
    );
};

export default AddExtComponent;