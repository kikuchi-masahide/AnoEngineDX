import React from 'react';
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
} from 'chart.js';
import { Line, getElementAtEvent } from 'react-chartjs-2';
import { useState } from 'react';

ChartJS.register(
    CategoryScale,
    LinearScale,
    PointElement,
    LineElement,
    Title,
    Tooltip,
    Legend
  );

import { useRef } from 'react';

//点と点の間の表示距離
const widthperpoint = 18;

//jsonのデータを受け取り、それを描画する線グラフ(<Line/>)を返す
function GetLines(jsondata,chartRef,onClick){
    const datas = jsondata['datas'];
    let labels = new Array();
    datas.forEach((d)=>{
        let name = d['counter'];
        if(d['logs'].length > 0){
            name = name+"*";
        }
        labels.push(name);
    });
    let dataset = new Array(2);
    dataset[0] = {
        label:'update_time',
        data:datas.map((d)=>d['update_time']),
        borderColor:'rgb(255, 99, 132)',
        backgroundColor:'rgb(255, 99, 132)',
        xAxisID:'xx'
    };
    dataset[1] = {
        label:'output_time',
        data:datas.map((d)=>d['output_time']),
        borderColor:'rgb(53, 162, 235)',
        backgroundColor:'rgb(53, 162, 235)',
        xAxisID:'xx'
    };
    let data = {
        labels:labels,
        datasets:dataset
    };
    let dataset2 = new Array(3);
    dataset2[0] = {
        label:'obj_num',
        data:datas.map((d)=>d['obj_num']),
        borderColor:'rgb(255, 99, 132)',
        backgroundColor:'rgb(255, 99, 132)'
    };
    dataset2[1] = {
        label:'update_comp_num',
        data:datas.map((d)=>d['update_comp']),
        borderColor:'rgb(53, 162, 235)',
        backgroundColor:'rgb(53, 162, 235)'
    };
    dataset2[2] = {
        label:'output_comp_num',
        data:datas.map((d)=>d['output_comp']),
        borderColor:'rgb(52, 235, 70)',
        backgroundColor:'rgb(52, 235, 70)'
    };
    let data2 = {
        labels:labels,
        datasets:dataset2
    };
    const style={
        overflow:"scroll"
    };
    const opt = {
        maintainAspectRatio:false,
        responsive:false,
        interaction:{
            mode:'index',
            intersect:false
        },
        scales:{
            xx:{
                ticks:{
                    autoSkip:false
                }
            }
        },
        stacked:false,
        plugins:{
            title:{
                display:true,
                text:'update_time/output_time'
            }
        },
        elements:{
            point:{
                hitRadius:5
            }
        }
    };
    return(
        <div style={style}>
            <Line options={opt} data={data} width={""+(widthperpoint*datas.length+120)} height="400" ref={chartRef} onClick={onClick}/>
            <p className='fs-5'>average update time:{jsondata['avupdatetime']}</p>
            <p className='fs-5'>average output time:{jsondata['avoutputtime']}</p>
            <Line options={opt} data={data2} width={""+(widthperpoint*datas.length+120)} height="400" ref={chartRef} onClick={onClick}/>
        </div>
    )
}

//onClickの結果を受け取り、その時間のログを表示する(ログがないタイミングならば変更しない)
function UpdateLogs(event,datas,setlogs){
    if(event.length === 0){
        return;
    }
    const index = event[0].index;
    //当該添え字のログが存在しなければ変更しない
    if(datas[index]['logs'].length > 0){
        setlogs({
            index:index,
            logs:datas[index].logs
        });
    }
}

export default function ChartArea(props){
    const jsondata = props.jsondata;
    const filenum = jsondata['filenum'];
    const filename = jsondata['filename'];
    if(jsondata === -1){
        return (<p>no data</p>)
    };
    //logs.index:表示するインデックス
    //logs.logs:表示する文字列の配列
    const [logs,setlogs] = useState({
        index:-1,
        logs:new Array()
    });
    const chartRef = useRef();
    const onClick = (event)=>{
        UpdateLogs(getElementAtEvent(chartRef.current,event),jsondata['datas'],setlogs);
    };
    return (
        <div className='container'>
            <h1 className='h1'>filename:{filename}</h1>
            <h1 className='h1'>filenum:{filenum}</h1>
            {GetLines(jsondata,chartRef,onClick)}
            <h2 className='h2'>{logs.index>=0?"counter:"+jsondata['datas'][logs.index]['counter']:""}</h2>
            {logs.logs.map((str,key)=><p key={key}>{str}</p>)}
        </div>
    );
}