import React,{ FC } from "react";
import ShowExcludeFileUnitComponent from "./ShowExcludeFileUnitComponent";

type ShowExcludeFileComponentPropsType = {
    files:Array<string>,
    //削除ボタンを押した際のonClick
     onClick:(index:number) => void
};

const ShowExcludeFileComponent:FC<ShowExcludeFileComponentPropsType> = (props) => (
    <div className='mt-3 mb-3'>
        <label className='form-label'>除外ファイル</label>
        <div className='container'>
        {
            props.files.map((file:string, index:number) => <ShowExcludeFileUnitComponent exclude={file} onClick={props.onClick} index={index}/>)
        }
        </div>
    </div>
)

export default ShowExcludeFileComponent;
