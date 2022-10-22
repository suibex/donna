"""
    *part of nitrodegen's god tool for debugging aarch64 based systems.*
        
        web-server module
            its going to run all of the greatest technology for your 'perfect' debugging.
            (this code is ass)
            
        (C) Gavrilo Palalic 2022.
"""
import donna_backend
import os,io,sys
import struct 
from flask import * 
app = Flask(__name__,static_folder='./static')
@app.route('/',methods=['GET','POST'])
def main():
    return render_html("index.html")
app.run("0.0.0.0",3002,debug=True)#qira basic shit

