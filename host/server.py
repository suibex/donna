"""
*******************************************************************************************************************************************

		donna disassembler v1.1
			-web server module

		Last update commited at: Mar 21 22.

		Created by nitrodegen.
		In labs of cyfly Computer Corporation.

	WARNING: ANY USE OF THIS CODE FOR PATENT OR PROFITABLE USE OR REUPLOAD OF SOURCE IS STRONGLY RESTIRCED, AND WILL BE HELD UNDER LAW!
	NOTE: ONLY USE THIS CODE TO CHANGE AND UPDATE THE BRANCH FOR THE MAIN REPO, NEVER FOR YOURSELF!

******************************************************************************************************************************************
"""
from flask import Flask,render_template,request, jsonify,redirect,url_for
import os ,io,sys
import logging
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

dis=sys.argv[1]

dir1 = os.listdir("/var/tmp/donna")
hexes=[]
dumps=[]
reg=[]
for file in dir1:
    if(".dump" in file):
        dumps.append(file)

    if(".hex" in file):
        hexes.append(file)
    if(".reg" in file):
        reg.append(file)


app = Flask(__name__,template_folder='content')

@app.route("/")
def main():
    return render_template("index.html")


@app.route("/main",methods=["GET","POST"])
def da2():
    file = sys.argv[1]+".gpd"
    d = open(f"/var/tmp/donna/{file}","r")
    data = d.read().split("\n")
    return jsonify(data)
    

@app.route("/hex",methods=["GET","POST"])
def hex():
    data=[]
    for h in hexes:
        f = open(f"/var/tmp/donna/{h}","r")
        cont = f.read()
        
        data.append(cont)
        f.close()

    return jsonify(data)



@app.route("/regs",methods=["GET","POST"])
def regs():
    cont=[]
    for r in reg:
        d = open(f"/var/tmp/donna/{r}","r")
        data = d.read()
        
        cont.append(data)

    return jsonify(cont)
   


@app.route("/dumps",methods=["GET","POST"])
def da1():
    data=[]
    for dump in dumps:
        f = open(f"/var/tmp/donna/{dump}","r")
        cont = f.read()
        cont = cont.split("\n")
        coci=""
        for coc in cont:
            coc+="</br>"
            coci+=coc

        data.append(coci)
        f.close()

    return jsonify(data)


app.run("0.0.0.0",80,debug=True)
