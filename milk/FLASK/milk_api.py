from turtle import clear
from flask import *
App = Flask(__name__)
App.config['debug']=True
import mysql.connector as mysql
db=mysql.connect(
    host="localhost",
    user='root',
    passwd="",
    database ="milk_grade"
)
cursor =db.cursor()

global phData
global temperatureData
global tastesData
global odeurData
global fatData
global turbidityData
global colorData


@App.route("/insert/data",methods =['GET'])
def home():
    global PH
    global Temperature
    global Tastes
    global Odeur
    global fat
    global turbidity
    global Color
    if 'ph' in request.args and 'temp' in request.args and 'taste' in request.args and 'ode' in request.args and 'fa' in request.args and 'turbi'in request.args and 'col' in request.args:
        PH=float(request.args.get('ph'))
        Temperature =int(request.args.get('temp'))
        Tastes =bool(request.args.get('taste'))
        Odeur=bool(request.args.get('ode'))
        fat =bool(request.args.get('fa'))
        turbidity =bool(request.args.get('turbi'))
        Color =int(request.args.get('col'))
        querry = "INSERT INTO milk_tb(pH,Temprature,Taste,Odor,Fat,Turbidity,Colour) VALUES ('%s','%s','%s','%s','%s','%s','%s')"
        values = (PH,Temperature,Tastes,Odeur,fat,turbidity,Color)
        cursor.execute(querry,values)
        db.commit()
        return "Data are inserted into Mysql Database Successfully"
#@App.route("/sensors/dashboard")
#def display():
#    import pickle
 #   with open("milk_prediction.pkl","rb") as f:
#        model = pickle.load(f)
 #   moisture = model.predict([[PH,Temperature,Tastes,Odeur,Fat,Turbidity,Color]])
 #   return render_template('milk_display.html',ph=PH,temp=Temperature,taste=Tastes,ode=Odeur,fat=Fat,turbi=Turbidity,col=Color,pred=Grade)

@App.route("/sensors/dashboard")
def display():
    try:
        import pickle
        with open("milk_prediction_model.pkl", "rb") as f:
            model = pickle.load(f)
        quality = model.predict([[6.6, 35.0, 0, 1,0,1,254]])
        query = cursor.execute(
            "SELECT pH,Temprature,Taste,Odor,Fat,Turbidity,colour, AddedAt FROM SENSORS_READINGS WHERE AddedAt >= DATE_SUB(NOW(), INTERVAL 1 HOUR) ORDER BY id ASC")
        queryData = cursor.fetchall()
        query1Data = ""
        query2Data = ""
        query3Data = ""
        query4Data = ""
        query5Data = ""
        query6Data = ""
        query7Data = ""
        addedAtData = ""

        for row in queryData:
            query1Data += str(row[0]) + ","
            query2Data += str(row[1]) + ","
            query3Data += str(row[2]) + ","
            query4Data += str(row[3]) + ","
            query5Data += str(row[4]) + ","
            query6Data += str(row[5]) + ","
            query7Data += str(row[6]) + ","
            addedAtData += str(row[8]) + ","

        cursor.close
        db.close

        return render_template('milk_display.html', pH=6.6, Temprature=37, Taste=0, Odor=1,fat=0,turbidity=1,Color=254, pred=quality, phData=query1Data, temperatureData=query2Data, tastesData=query3Data, odeurData=query4Data, fatData=query5Data, turbidityData=query6Data,colorData=query7Data, AddedAtData=addedAtData)
    except:
        return render_template('milk_display.html', pH="-", Temprature="-", Taste="-", Odor="-",fat="_",turbidity="_",Color="_", pred="-", phData=0, temperatureData=0,tastesData=0, odeurData=0, fatData=0, turbidityDta=0,colorData=0,AddedAtData=0)


if __name__ =="__main__":
    App.run(debug=True,host='0.0.0.0')