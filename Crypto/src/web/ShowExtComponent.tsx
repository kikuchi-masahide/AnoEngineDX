import React,{FC} from 'react';
import ShowExtUnitComponent from './ShowExtUnitComponent';

type ShowExtComponentPropType = {
    exts:Array<[string,string]>,
    //削除ボタンを押した際のonClick
     onClick:(index:number) => void
};

const ShowExtComponent:FC<ShowExtComponentPropType> = (props) => {
    return(
        <div className='mt-3 mb-3'>
          <label className='form-label'>拡張子変換</label>
          <div className='container'>
            {
              props.exts.map((tpl:[string,string], index:number) => <ShowExtUnitComponent ext={tpl} index={index} onClick={props.onClick}/>)
            }
          </div>
        </div>
    )
};

export default ShowExtComponent;