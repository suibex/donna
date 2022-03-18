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
    const r = await req.json()
    var fin=""
    for(let i =0;i<r.length;i++){
        fin+=r[i]+="<br>"
    }
    var doc = document.getElementById("maintext").innerHTML=fin;
    
    
}
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

    var title = document.getElementById("where").innerHTML=r[0]+":";
    var doc = document.getElementById("rax").innerHTML=r[1];
    var doc1 = document.getElementById("rbx").innerHTML=r[2];
    var doc2= document.getElementById("rcx").innerHTML=r[3];
    var doc3 = document.getElementById("rdx").innerHTML=r[4];
    var doc4 = document.getElementById("rsi").innerHTML=r[5];
    var doc7 = document.getElementById("rdi").innerHTML=r[6];
   
    var doc6 = document.getElementById("rbp").innerHTML=r[7];
    var doc5 = document.getElementById("rip").innerHTML=r[8];
    var doc8 = document.getElementById("rsp").innerHTML=r[9];
    console.log("R:"+r[8])
   
    
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
    console.log(r)

    const divs=[]
    var size = 5;
    var doc = document.getElementById("mainfun")
    const svg1 = document.createElementNS("http://www.w3.org/2000/svg", "svg");
    svg1. setAttribute ("width", "10000" );
    svg1. setAttribute ("height", "600" );
    
    for(let i = 0;i<r.length;i++){
        
        if(r[i].length>25){
            
            var div = document.createElement('div');
        
        


            
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
            div.style.left=`${(size/29)*25}%`;
            div.style.top=`${size/40}%`;
            div.style.fontSize="0.5rem"
            
            div.innerHTML=r[i]
            

            div.style.color="black";
            div.style.fontFamily="monospace"
            console.log(`${size}%`);
            div.style.to
            div.className = 'ui-modal';
            document.body.appendChild(div);
            size+=25;
            var newLine = document.createElementNS('http://www.w3.org/2000/svg','line');
            newLine.setAttribute('id',`line${i}`);
            
            newLine.setAttribute('x1',doc.offsetLeft);
            newLine.setAttribute('y1',doc.offsetTop);
            console.log("W:"+div.clientWidth+"\nH:"+div.clientHeight)
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