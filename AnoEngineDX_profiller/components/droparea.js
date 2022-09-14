import { useEffect } from "react";

export default function DropArea(props){
    const setjsondata = props.setjsondata;
    const handleDropAction = (e)=>{
        e.preventDefault();
        e.stopPropagation();
        const droppedfile = e.dataTransfer.files;
        if(droppedfile.length == 0){
            return;
        }
        const file = droppedfile[0];
        const fileReader = new FileReader();
        fileReader.addEventListener('load',(load)=>{
            const json = JSON.parse(fileReader.result);
            json["filename"] = filename;
            console.log(json);
            //平均update/output時間を計算する
            let avupdatetime = 0;
            let avoutputtime = 0;
            json['datas'].forEach((val,_)=>{
                avupdatetime += val['update_time'];
                avoutputtime += val['output_time'];
            });
            avupdatetime /= json['datas'].length;
            avoutputtime /= json['datas'].length;
            json['avupdatetime'] = avupdatetime;
            json['avoutputtime'] = avoutputtime;
            setjsondata(json);
        });
        const filename = file.name;
        fileReader.readAsText(file);
    };
    const onload = ()=>{
        console.log("onload");
        const droparea = document.getElementById('droparea');
        ['drag','dragstart','dragend','dragover','dragenter','dragleave'].forEach(event=>{
            droparea.addEventListener(event,(e)=>{
                e.preventDefault();
                e.stopPropagation();
            });
        });
        droparea.addEventListener('drop',handleDropAction);
    };
    useEffect(onload,[]);
    return(
        <div id="droparea" className="container">
            <h1>drop profile data here</h1>
        </div>
    )
}