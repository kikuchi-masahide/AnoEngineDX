import React,{ FC } from 'react';

type ExtComponentPropsType = {
    //表示する変換前後の拡張子
    ext:[string,string],
    //削除ボタンを押した際のonClick
    onClick:(index:number) => void,
    //これが配列extsの何番目の拡張子か
    index:number
};

const ShowExtUnitComponent:FC<ExtComponentPropsType> = (props) => (
    <div className='input-group mb-1' key={props.index}>
        <span className='input-group-text col-3'>{props.ext[0]}</span>
        <span className='input-group-text col-1'> to </span>
        <span className='input-group-text col-3'>{props.ext[1]}</span>
        <div className='col-1'></div>
        <button className='btn btn-secondary col-1' onClick={() => props.onClick(props.index)}>消去</button>
    </div>
);

export default ShowExtUnitComponent;