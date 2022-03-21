# donna

<b>debugger often named "never again"</b> (donna) is a debugger and analyzer made for ELF executables in Linux.
<br>Competitor to gdb or lldb<br>
<br>Created by me , so feel free to change and modify it , as long as you don't sell it ,copy it,etc...<br>
<h4>i'm not into you, i'm donna. </h4>
<br>donna provides you with <b>webserver</b> that you can connect to with localhost:80 
  <br>also donna provides you with a backend shell <b>(donna shell)</b> to set and use breakpoints</br>

![Screenshot from 2022-03-18 15-35-44](https://user-images.githubusercontent.com/59802817/159026803-a02f9757-a996-46d2-84ec-f9da0c7236db.png)


<br>if you are not a fan of webservers, you can just use terminal version.</br>
<h1>Example:</h1>

![Screenshot from 2022-03-18 15-34-21](https://user-images.githubusercontent.com/59802817/159027006-23768ace-15e9-4795-879a-e655f2c507dd.png)
<h3>code that was disassembled, but in C:<h3>

>#include <stdio.h>
  <br>int main(){
    <br>int a = 25;
    <br>printf("\nA is %d\n",a);
    <br>if(a>0)
    <br>  printf("\nA is positive.\n");
    <br>else
    <br>    printf("\nA is negative.\n");
  }


<br><b>you can also follow traces , branches , etc with donna , as long as it is not too big (max 25 traces) :)</b> </br>
![Screenshot from 2022-03-21 20-14-28](https://user-images.githubusercontent.com/59802817/159347493-1d5d7515-86e3-4d7f-9cd9-b67047fd7694.png)

  
<h1>Timeless debugging, no need to set breakpoints.<h1>
  
  <br><b>Just press arrow-up to move up a instruction breakpoint , and arrow-down to go back!</b></br>
![Screenshot from 2022-03-21 20-20-22](https://user-images.githubusercontent.com/59802817/159348174-819172ae-9f4e-40e8-950f-ce5684fcfc82.png)


<h3>Limitations of donna:</h3>
<br>well , i didn't really put too much time into this , so it could be used for basic reverse engineering, but it works kind of ok tbh.
<br>feel free to help me make <b>donna</b> great by contributing updates and fixes!</br>
  <br><b>donna only works on debian Linux , more preferably , Ubuntu 18.04 and higher!</b></br>


<h1>manual:<h1>
  
  ![Screenshot from 2022-03-18 15-34-01](https://user-images.githubusercontent.com/59802817/159028109-ed045e95-8079-4e8e-b94f-9134f3bf07c3.png)
  <br><b>Build donna with setup.sh script.</b></br>
<h2>Techonologies used to create donna</h2>
  <br>I used C++ for donna
  <br>my techniques that i learned are used to actually disassemble elf files(not the best choice , but had no other option)
  <br>im using ptrace for tracing the process ,setting the breakpoint with PTRACE_PEEKDATA and PTRACE_POKEDATA
   <br>And used python3 ,HTMl,CSS for web.
    <br>ofc , flask runs the server. (no hassle)
 
  <h3>have a nice day.</h3>
  
  
  
  
  
