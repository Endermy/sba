#include <string>
#include <iostream>
#include "html.h"

std::string getIndexHTML(std ::string Nodelist)
{
    std::string content;
    content = R"a(
    
<!DOCTYPE html>
<html>
    <head> 
        <meta charset="utf-8">
        <title>sba html</title>
        <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@20..48,100..700,0..1,-50..200" />
        <link rel="stylesheet" href="index.css" />
        <!-- -------------------------------------------------- -->
        <script>
            nodes=[)a";
    content += Nodelist;
    content += R"a(]
        
        var NodeSelected="None";

        function changeImage2Index(){
            document.getElementById("graph").src="/status.html";
            document.getElementById("graph").style.backgroundColor="black";
            document.getElementById("status").innerHTML="now on : index";
        }
        function changeImage2airPressure(){
            document.getElementById("graph").style.backgroundColor="white";
            if(NodeSelected!="None"){
                document.getElementById("graph").src="/GetGraph?DATAname=airPressure&Nodename="+NodeSelected;
                document.getElementById("status").innerHTML="now on : "+NodeSelected+"|airPressure";
            }else{
                alert("None of node selected");
            }
        }
        function changeImage2lumen(){
            document.getElementById("graph").style.backgroundColor="white";
            document.getElementById("graph").style.backgroundColor="white";
            if(NodeSelected!="None"){
                document.getElementById("graph").src="/GetGraph?DATAname=lumen&Nodename="+NodeSelected;
                document.getElementById("status").innerHTML="now on : "+NodeSelected+"|lumen";
            }else{
                alert("None of node selected");
            }
            
        }
        function changeImage2humidity(){
            document.getElementById("graph").style.backgroundColor="white";
            if(NodeSelected!="None"){
                document.getElementById("graph").src="/GetGraph?DATAname=humidity&Nodename="+NodeSelected;
                document.getElementById("status").innerHTML="now on : "+NodeSelected+"|humidity";
            }else{
                alert("None of node selected");
            }

        }
        function nodeOnClick(){
            NodeSelected=event.srcElement.id;
            var _temp =document.getElementsByClassName("nodelist")[0];
            for(var i = 1; i < _temp.children.length; i++){
                _temp.children[i].className="node0";
            }
            document.getElementById(NodeSelected).className="node1";
        }
        function Nodelist(){
            for (var i=0;i<nodes.length;i++){
                document.write(
                    `<a
                        onclick="nodeOnClick()"
                        class="node0"
                        id="${nodes[i]}">
                        <div class="nodeName"
                            id="${nodes[i]}">${nodes[i]}</div>
                    </a>`
                );
            }
        }
        
    </script>

        <!-- -------------------------------------------------- -->

        <body>
            <div class="header">
                <a href="setup.html" class="rightheader">
                    <span class="material-symbols-outlined">
                        settings
                    </span></a>
            </div>
            <div class="box">
                <!-- --------------------------------------------------------------------- -->
                <div class="column">
                    <div class="leftpanel">
                        <div class="title">Node list</div>
                        <div class="nodelist">
                            <!-- -------------------------------------- -->
                            <script>
                                Nodelist();
                            </script>
                            <!-- -------------------------------------- -->
                        </div>
                    </div>
                </div>
                <!-- --------------------------------------------------------------------- -->
                <div class="column">
                    <div class="graphstatus">
                        <div class="text" id="status">now on : index</div>
                    </div>
                    <iframe
                        class="webview"
                        id="graph"
                        src="/status.html"></iframe>
                    <div class="buttonlist">
                        <a
                            class="graphbutton"
                            onclick="changeImage2Index()">Index</a>
                        <a
                            class="graphbutton"
                            onclick="changeImage2airPressure()">airPressure</a>
                        <a
                            class="graphbutton"
                            onclick="changeImage2lumen()">lumen</a>
                        <a
                            class="graphbutton"
                            onclick="changeImage2humidity()">humidity</a>
                    </div>
                </div>
                <!-- --------------------------------------------------------------------- -->
                <div class="column"></div>
                <!-- --------------------------------------------------------------------- -->
            </div>
            <div class="bottom">
                <div class="text">*Build by Fukien Secondary School 6B 22 Chung
                    Toi Lam for SBA,2024年7月12日</div>
            </div>
        </body>
    </html>
    )a";
    return content;
}
std::string getGraphContent(std::string x, std::string y)
{
    std::string content;
    content +=
        R"a(<!DOCTYPE html>
<html lang="en" style="height: 100%">
  <head>
    <meta charset="utf-8">
  </head>
  <body style="height: 100%; margin: 0">

    <!-- ---------------------------------------------------------------- -->
    <div id="container" style="height: 100%"></div>
    <div id="alert"
      style="visibility: hidden; position: fixed; background-color: red; color: white; left: 40%; top: 45%; width: 20%; height: 10%; text-align: center; line-height: 80px;"><div
        class="alerttext">No data!</div></div>
    <!-- ---------------------------------------------------------------- -->

    <script type="text/javascript"
      src="https://cdn.jsdelivr.net/npm/echarts@4.3.0/dist/echarts.min.js"></script>
    <script type="text/javascript">
    var dom = document.getElementById('container');
    var myChart = echarts.init(dom, null, {
      renderer: 'canvas',
      useDirtyRect: false
    });
    var app = {};
    var option;
    option = {
      xAxis: {
        type: 'category',
        data: [
)a";
    content += x;
    content += R"a(]
      },
      yAxis: {
        type: 'value'
      },
      series: [
        {
          data: [)a";
    content += y;
    content += R"a(],
          type: 'line'
        }
      ]
      };

if(option.xAxis.data[0]!=null && option.series[0].data[0]!=null){  
  if (option && typeof option === 'object') {
      myChart.setOption(option);
    }
    window.addEventListener('resize', myChart.resize);
  }else{
    document.getElementById("alert").style.visibility="visible";
  }
  </script>
  </body>
</html>

)a";
    return content;
}
std::string getIndexCSS()
{
    std::string returnval =
        R"(* {
    margin: 0px;
    padding: 0px;
    box-sizing: border-box;
    font-size: 24px;
}

.statusbox{
    box-sizing: border-box;
    height: 100%;
    .left {
        /* display: inline; */
        float: left;
        height: 100%;
        width: 50%;
        margin-right: 0px;
        background-color: black;
    }
    
    .right {
        /* display: inline; */
        float: right;
        height: 100%;
        width: 50%;
        background-color: black;
    }
}

.material-symbols-outlined {
    font-size: 50px;
    color: black;
    transition: 0.2s;

    font-variation-settings:
        'FILL' 0,
        'wght' 400,
        'GRAD' 0,
        'opsz' 48,
}

.material-symbols-outlined:hover {
    color: red;
    transition: 0.2s;

}

.border {
    border-style: solid;
    border-width: 1px;
}

body {
    height: 100vh;
    width: auto;
    flex-direction: column;
    align-items: stretch;
    display: flex;

    .inputview {
        height: 100vh;
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;

        .inputview2 {
            display: flex;
            justify-content: center;

            .return {
                display: inline-block;
                user-select: none;
                border-style: solid;
                border-width: 1px;
                border-radius: 3px;
                background-color: rgb(240, 240, 240);
                width: max-content;
                margin-top: 10px;
                margin-left: 10px;
                text-decoration: none;
                color: black;
            }
        }

    }

    .header {
        flex: 1;
        border-style: solid;
        border-width: 1px;
        position: relative;

        .rightheader {
            position: absolute;
            right: 0px;
            width: 5vh;
            height: 100%;
            /* background-image: url("setup.png"); */
            background-size: contain;
            background-repeat: no-repeat;
        }
    }

    .box {
        flex: 18;
        border-style: solid;
        border-width: 1px;
        /* -------------------------------------------- */
        display: flex;
        flex-direction: row;
        align-items: stretch;

        .column {
            flex: 1;

            .leftpanel {
                width: 100%;
                height: 100%;

                .nodelist {
                    flex-direction: column;
                    margin-left: 5%;
                    height: 100%;

                    .node1 {
                        display: flex;
                        height: 34px;
                        border-style: solid;
                        border-width: 1px;
                        border-radius: 5px;
                        width: 95%;
                        margin-bottom: 1px;

                        user-select: none;
                        text-decoration: none;
                        left: 0px;
                        align-items: center;
                        position: relative;

                        border-color: rgb(36, 187, 36);
                        background-image: linear-gradient(to right, rgb(161, 255, 161), rgb(36, 187, 36));

                        .nodeName {
                            margin-left: 20px;
                            display: inline-block;
                        }
                    }

                    .node0 {
                        display: flex;
                        height: 34px;
                        border-style: solid;
                        border-width: 1px;
                        border-radius: 5px;
                        width: 95%;
                        margin-bottom: 1px;

                        user-select: none;
                        text-decoration: none;
                        left: 0px;
                        align-items: center;
                        position: relative;

                        border-color: rgb(214, 60, 60);
                        background-image: linear-gradient(to right, rgb(255, 161, 161), rgb(214, 60, 60));

                        .nodeName {
                            margin-left: 10px;

                            display: inline-block;
                        }
                    }
                }

            }

            .title {
                height: 5%;
                padding-top: 3%;
                text-align: center;

            }

            .rightpanel {
                margin-top: .5rem;
                margin-left: .5rem;
                width: max-content;
                height: max-content;
            }

        }

        .column:nth-child(2) {
            flex: 5;
            display: flex;
            flex-direction: column;

            .graphstatus {
                flex: 1;
                border-style: solid;
                border-width: 1px;
                display: flex;
                justify-content: center;
                align-items: center;

                .text {
                    text-align: center;
                }
            }

            .webview {
                flex: 18;
                border-style: solid;
                border-width: 1px;
                width: 100%;
                background-color: black;
            }

            .buttonlist {
                flex: 1;
                border-style: solid;
                border-width: 1px;

                display: flex;

                .graphbutton {
                    user-select: none;
                    border-style: solid;
                    border-width: 1px;
                    text-align: center;
                    text-decoration: none;
                    flex: 1;
                    display: inline-block;
                    background-color: rgb(0, 0, 0);
                    color: rgb(255, 255, 255);
                    height: 100%;
                    display: flex;
                    justify-content: center;
                    align-items: center;

                    .buttontext {
                        flex: 1;
                    }
                }
            }
        }
    }

    /* -------------------------------------------- */

    .bottom {
        flex: 1;
        border-style: solid;
        border-width: 1px;
    }
}
    )";
    return returnval;
}
std::string getStatusHTML(){
    std::string returnval;
    returnval=
    R"a(<head><link rel="stylesheet" href="index.css" /></head>
<body>
    <div class="statusbox">
        <iframe src="/GetstatusLeft"
            frameborder="0"
            class="left"></iframe>
        <iframe src="/GetstatusRight"
            frameborder="0"
            class="right"></iframe>
    </div>
</body>)a";
    return returnval;
}
std::string getSetupHTML(){
    std::string returnval;
    returnval=
    R"a(<!DOCTYPE html>
<html lang="cn">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link rel="stylesheet" href="index.css" />

        <title>setup</title>
    </head>
    <body>
        <div class="inputview">
            <div class="inputview2">
                <form action="/postSetup" method="post" class="formdiv">
                    <label for="serverPort">服务器聆听端口:</label>
                    <input type="text" id="serverPort" name="serverPort"
                        required>
                    <br>
                    <label for="SQLserverIP">SQL服务器域名:</label>
                    <input type="text" id="SQLserverIP" name="SQLserverIP"
                        required>
                    <br>
                    <label for="SQLserverPort">SQL服务器端口:</label>
                    <input type="text" id="SQLserverPort" name="SQLserverPort"
                        required>
                    <br>
                    <label for="username">SQL服务器账号:</label>
                    <input type="text" id="username" name="username"
                        required>
                    <br>
                    <label for="password">SQL服务器密码:</label>
                    <input type="text" id="password" name="password"
                        required>
                    <br>
                    <input type="submit" value="提交" class="inputbutton">
                    <a class="return" href="/">返回主页</a>
                    

                </form>
            </div>
        </div>
    </body>
</html>)a";
    return returnval;
}
