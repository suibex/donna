async function readMain(){
    
    const shit=''
    const shit2 = ''
    const shit3 = '' 
    const shit4 = '' 
    const data={shit};
    const options= { 
        method: 'POST',
        headers:{
            'Content-Type':'application/json'
        },
        body:JSON.stringify(data)
    };
    
    const req = await fetch('/main',options)
    let r = await req.json();
 
    let cloned = r 
    var fin=""
    for(let i =0;i<r.length;i++){
        fin+=r[i]+="<br>"
    }
    
   
  
    var doc = document.getElementById("maintext").innerHTML=fin;

};

let i =0;
async function readDump(){
    
    const shit=''
    const shit2 = ''
    const shit3 = '' 
    const shit4 = '' 
    const data={shit};
    const options= { 
        method: 'POST',
        headers:{
            'Content-Type':'application/json'
        },
        body:JSON.stringify(data)
    };
    
    const req = await fetch('/hex',options)
    const r = await req.json()


    var doc = document.getElementById("dump").innerHTML=r;
    
    
}
async function readRegs(){
    
    const shit=''
    const shit2 = ''
    const shit3 = '' 
    const shit4 = '' 
    const data={shit};
    const options= { 
        method: 'POST',
        headers:{
            'Content-Type':'application/json'
        },
        body:JSON.stringify(data)
    };
    
    const req = await fetch('/regs',options)
    const r = await req.json()
    
    
    /*
    var doc = document.getElementById("rax").innerHTML=r[1];
    var doc1 = document.getElementById("rbx").innerHTML=r[2];
    var doc2= document.getElementById("rcx").innerHTML=r[3];
    var doc3 = document.getElementById("rdx").innerHTML=r[4];
    var doc4 = document.getElementById("rsi").innerHTML=r[5];
    var doc7 = document.getElementById("rdi").innerHTML=r[6];
   
    var doc6 = document.getElementById("rbp").innerHTML=r[7];
    var doc5 = document.getElementById("rip").innerHTML=r[8];
    var doc8 = document.getElementById("rsp").innerHTML=r[9];
    */
   // console.log("R:"+r[8])
  
    //console.log(r[0])
    
   document.onkeydown = function(e) {

        console.log(r[i])
        if(r[i] != undefined){

            let stuff = r[i].split("\n");

                
            switch (e.keyCode) {
                
                case 38:
                    
                    
                    var title = document.getElementById("where").innerHTML=stuff[0]+":";
                    var doc = document.getElementById("rax").innerHTML=stuff[1];
                    var doc1 = document.getElementById("rbx").innerHTML=stuff[2];
                    var doc2= document.getElementById("rcx").innerHTML=stuff[3];
                    var doc3 = document.getElementById("rdx").innerHTML=stuff[4];
                    var doc4 = document.getElementById("rsi").innerHTML=stuff[5];
                    var doc7 = document.getElementById("rdi").innerHTML=stuff[6];
                
                    var doc6 = document.getElementById("rbp").innerHTML=stuff[7];
                    var doc5 = document.getElementById("rip").innerHTML=stuff[8];
                    var doc8 = document.getElementById("rsp").innerHTML=stuff[9];
                   
                    
                    
                    if(r.length>i){
                        i++;
                    }
                    
                    break;
            
                
            
            case 40:
                    
                    
                    var title = document.getElementById("where").innerHTML=stuff[0]+":";
                    var doc = document.getElementById("rax").innerHTML=stuff[1];
                    var doc1 = document.getElementById("rbx").innerHTML=stuff[2];
                    var doc2= document.getElementById("rcx").innerHTML=stuff[3];
                    var doc3 = document.getElementById("rdx").innerHTML=stuff[4];
                    var doc4 = document.getElementById("rsi").innerHTML=stuff[5];
                    var doc7 = document.getElementById("rdi").innerHTML=stuff[6];
                
                    var doc6 = document.getElementById("rbp").innerHTML=stuff[7];
                    var doc5 = document.getElementById("rip").innerHTML=stuff[8];
                    var doc8 = document.getElementById("rsp").innerHTML=stuff[9];
                    console.log(i)
                    
                    
                    if(i>=0){
                        i--;
                        console.log(i)
                    }
                    
                    break;
            
                
            }            
        }
        else{
            if(i>=0){
                i--;
            }
            else{
                i++;
            }
        }
       
   }
   


   
   
   
    
}
async function readTraces(){
    
    const shit=''
    const shit2 = ''
    const shit3 = '' 
    const shit4 = '' 
    const data={shit};
    const options= { 
        method: 'POST',
        headers:{
            'Content-Type':'application/json'
        },
        body:JSON.stringify(data)
    };
    
    const req = await fetch('/dumps',options)
    const r = await req.json()
    //console.log(r)

    const divs=[]
    var size = 5;
    var size1 = 5;
    var doc = document.getElementById("mainfun")
    const svg1 = document.createElementNS("http://www.w3.org/2000/svg", "svg");
    svg1. setAttribute ("width", "10000" );
    svg1. setAttribute ("height", "600" );
    let times = 0;
    for(let i = 0;i<r.length;i++){
        
        if(r[i].length>25){
            
            var div = document.createElement('div');
        
        
            if(times==6){
                size1+=1300;
                size=10;
            }
            if(times==12){
                size1=60;
            }
           
            div.style.backgroundColor='white';
            div.style.position='absolute';
            div.style.width='15vh';
        
            div.innerHTML.position="absolute";
            div.innerHTML.left="10rem"
            div.style.paddingTop="10px"
            div.style.paddingLeft="10px"
            div.style.paddingRight="10px"
            div.style.paddingBottom="10px"
            div.style.border='5px';
            div.style.borderRadius='5%';
            div.style.left=`${(size/40)*20+20}%`;
            div.style.top=`${size1/38}%`;
            div.style.fontSize="0.5rem"
            
            div.innerHTML=r[i]
            

            div.style.color="black";
            div.style.fontFamily="monospace"
           // console.log(`${size}%`);
            div.style.to
            div.className = 'ui-modal';
            document.body.appendChild(div);
            size+=25;
            size1+=25;
            times++;
            var newLine = document.createElementNS('http://www.w3.org/2000/svg','line');
            newLine.setAttribute('id',`line${i}`);
            
            newLine.setAttribute('x1',doc.offsetLeft+200);
            newLine.setAttribute('y1',doc.offsetTop+200);
          //  console.log("W:"+div.clientWidth+"\nH:"+div.clientHeight)
            newLine.setAttribute('x2',div.offsetLeft);
            newLine.setAttribute('y2',div.offsetTop);
            newLine.setAttribute('stroke-length',2);
            if(i%2==0){
                newLine.setAttribute("stroke", "cyan")
            }
            else if(i%3==0){
                newLine.setAttribute("stroke", "yellow")
            }
            else{
                newLine.setAttribute("stroke", "red")
            }
            
            svg1.append(newLine)
        }
       
       
    }
    document.getElementById ('fuck'). appendChild ( svg1 );
    
}


readMain()
readTraces()
readDump()
readRegs()
