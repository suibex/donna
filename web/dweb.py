"""
    *part of nitrodegen's god tool for debugging aarch64 based systems.*
        
        web-server module
            its going to run all of the greatest technology for your 'perfect' debugging.
            (this code is ass)
            
        (C) Gavrilo Palalic 2022.
"""
import os,io,sys
import struct 
from flask import *
data = []
markings=""
app = Flask(__name__,static_folder='./static')
class DonnaWebServer(object):
    def __init__(self):
        
        app = Flask(__name__,static_folder='./static')
    def run_server(self):
        app.run("0.0.0.0",3001,debug=True)#qira basic shit
    @app.route('/',methods=['GET','POST'])
    def main():
        return render_template("index.html")
    @app.route('/file_request',methods=['GET','POST'])
    def send_file_data():
        #data=[data[0],data[1],data[2],markings]

        return jsonify(data)
   


