import React,{ FC } from "react";

type ShowExcludeFileUnitComponentPropsType = {
    //表示する除外ファイル名
    exclude:string,
    //削除ボタンを押した際のonClick
    onClick:(index:number) => void,
    //これが配列extsの何番目の拡張子か
    index:number
};

const ShowExcludeFileUnitComponent:FC<ShowExcludeFileUnitComponentPropsType> = (props) => (
    <div className='input-group mb-1' key={props.index}>
        <span className='input-group-text col-6'>{props.exclude}</span>
        <div className='col-1'></div>
        <button className='btn btn-secondary col-1' onClick={() => props.onClick(props.index)}>消去</button>
    </div>
);

export default ShowExcludeFileUnitComponent;
