from flask import *
App = Flask(__name__)
App.config['debug']=True
import mysql.connector as mysql
db=mysql.connect(
    host="localhost",
    user='root',
    passwd="",
    database ="milk_db"
)
cursor =db.cursor()
@App.route("/insert/data",methods =['GET'])
def home():
    global Temperature
    global Humidity
    if 'temp' in request.args and 'hum' in request.args:
        Temperature=float(request.args.get('temp'))
        Humidity =float(request.args.get('hum'))
        querry = "INSERT INTO DHT_TB(temperature,humidity) VALUES('%s','%s')"
        values = (Temperature,Humidity)
        cursor.execute(querry,values)
        db.commit()
        return "Data are inserted into Mysql Database Successfully"
@App.route("/sensors/dashboard")
def display():
    import pickle
    with open("moisture_model","rb") as f:
        model = pickle.load(f)
    moisture = model.predict([[Temperature,Humidity]])
    return render_template('aceiot_display.html',temp=Temperature,hum=Humidity,pred=moisture)

if __name__ =="__main__":
    App.run(debug=True,host='0.0.0.0')