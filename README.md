# donna
<p>
<b>debugger often named "never again"</b> (donna) is a debugger and analyzer made for ARM ELF executables that runs on Linux.
<br>Not a competitor to gdb or lldb or qira.<br>
<br>Created by me , so feel free to change and modify it , as long as you don't sell it ,copy it,etc...<br>
<h4>'i'm not into you, i'm donna.' - Suits </h4>
<br><p>donna provides you with <b>webserver</b> that you can connect to by localhost, port 3001</p>
<br>Not a fan of websites? , you have 'backend' executable so you can run donna without webserver (but why?)
</p>
 <h1>Timeless debugging , no need to set breakpoints.</h1>

  ![Screenshot 2022-10-23 at 17 56 05](https://user-images.githubusercontent.com/59802817/197403075-d7384e4c-1d60-43e3-a9a8-ac13699a20b4.png)



 <h2>donna in Safari ( backend running on docker): </h2>

  ![Screenshot 2022-10-23 at 17 54 47](https://user-images.githubusercontent.com/59802817/197403040-2136d0c8-a8b5-4115-83c0-1c486cf4cc86.png)

  <h3>Limitations of donna:</h3>
<p>
<br>well , donna does support banking ( you can load your custom instructions by .bank file ), it isn't documented. so , feel free to reverse engineer that.<br>also, donna uses ptrace (macOS hates ptrace. so debugging part of donna, doesn't work on macOS).
<br><b>I do not consider donna a usable tool for some professional reverse engineering. this is just my shot at building a debugger.</b>
<br>feel free to help me make <b>donna</b> great by contributing updates and fixes!</br>
  <br><b>donna only works on debian Linux , more preferably , Ubuntu 18.04 and higher!</b></br>
  </p>
  
<h1>how to build donna?</h1>
<br>to build donna. just run 'build_all.sh' and you're done. easy.<br>


  <h3>have a nice day.</h3>
  
  
  
  
  
