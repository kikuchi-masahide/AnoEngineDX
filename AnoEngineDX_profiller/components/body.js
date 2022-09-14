import { useState } from "react";
import DropArea from "./droparea";
import ChartArea from "./chartarea";

export default function Body(){
    const [jsondata,setjsondata] = useState(-1);
    return(
        <div className="container">
            {
                jsondata === -1?<DropArea setjsondata={setjsondata}/>:<ChartArea jsondata={jsondata}/>
            }
        </div>
    );
}