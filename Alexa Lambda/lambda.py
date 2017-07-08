### source: https://github.com/alexa/alexa-smarthome-validation/edit/master/python/lambda.py ###
import urllib
import json
import math
import logging

logger = logging.getLogger()
logger.setLevel(logging.INFO)
ngrok = "https://beaaa1b0.ngrok.io"
actors ={}
def lambda_handler(event, context):

    if str(event['header']['namespace']) == 'Alexa.ConnectedHome.Discovery':
        return handleDiscovery(context, event)

    elif str(event['header']['namespace']) == 'Alexa.ConnectedHome.Control':
        return handleControl(context, event)
		

def handleDiscovery(context, event):
    global ngrok
    global actors
    payload = ''
    header = {"namespace": "Alexa.ConnectedHome.Discovery", "name": "DiscoverAppliancesResponse", "payloadVersion": "2" }

    if event['header']['name'] == 'DiscoverAppliancesRequest':
    	myactors = urllib.urlopen(ngrok + "/SHP-Web/rest/gui/thing/actor/a/a").read()
    	actors = json.loads(myactors)
    	payload = {"discoveredAppliances":[]}
    	digitalFunctions = ["turnOn", "turnOff", "setColor" ]
    	analogFunctions = ["turnOn", "turnOff",	"setTargetTemperature", "setPercentage"]

    	for actor in actors:
    		function =[]
    		if(actor["type"] == str("DigitalActor")):
    		    function = digitalFunctions
    		else:
    		    function = analogFunctions
				
    		payload["discoveredAppliances"].append(		{		
    					"applianceId":str(actor["id"]),
    					"manufacturerName":"User Super Cooler Namer",
    					"modelName":str(actor["id"]),
    					"version":"1.0",
    					"friendlyName":str(actor["name"]),
    					"friendlyDescription":"Mein Super Cooles Lambda Geraet",
    					"isReachable":True,
    					"actions":function,
    				})

    return { 'header': header, 'payload': payload }


    
def handleControl(context, event):
    global ngrok
    
    payload = ''
    device_id = event['payload']['appliance']['applianceId']
    message_id = event['header']['messageId']
    actor = getActor(device_id)
    

    if event['header']['name'] == 'TurnOnRequest':
        if actor['type'] == "AnalogActor":
            urllib.urlopen(ngrok + "/SHP-Web/rest/gui/thing/" + str(device_id) + "/100/a/a"  ).read()
        else:
            urllib.urlopen(ngrok + "/SHP-Web/rest/gui/thing/" + str(device_id) + "/1/a/a"  ).read()            
        payload = { }
        header = generateResponseHeader(event, "TurnOnConfirmation")
    
    if event['header']['name'] == 'TurnOffRequest':
        urllib.urlopen(ngrok + "/SHP-Web/rest/gui/thing/" + str(device_id) + "/0/a/a"  ).read()
        payload = {}
        header = generateResponseHeader(event, "TurnOffConfirmation")
        
		
    if event['header']['name'] == 'SetColorRequest':
        rgb = getRGB(event)
        urllib.urlopen(ngrok + "/SHP-Web/rest/gui/thing/" + str(device_id) + "/" + str(rgb)  + "/a/a"  ).read()
        return generateLightingResponse(event)
        
    if event['header']['name'] == 'SetTargetTemperatureRequest': 
        target_temperature = event['payload']['targetTemperature']['value']
        urllib.urlopen(ngrok + "/SHP-Web/rest/gui/thing/" + str(device_id) + "/" + str(target_temperature) +"/a/a"  ).read()
        return  generateTemperatureResponse(event,target_temperature)
    if event['header']['name'] == 'SetPercentageRequest': 
        value = event['payload']['percentageState']['value']
        urllib.urlopen(ngrok + "/SHP-Web/rest/gui/thing/" + str(device_id) + "/" + str(value) +"/a/a"  ).read()
        header = generateResponseHeader(event, "SetPercentageConfirmation")
        logger.info('setPer')
        payload = {}
       # return  generateTemperatureResponse(event,value)
    
        
        

    return { 'header': header, 'payload': payload }
	
	
	
def generateResponseHeader(request,response_name):
    header = {
        'namespace': request['header']['namespace'],
        'name': response_name,
        'payloadVersion': '2',
        'messageId': request['header']['messageId'],        
    }
    return header

def generateResponse(header,payload):
    response = {
        'header': header,
        'payload': payload,
    }
    return response

def generateTemperatureResponse(request,target_temperature):
    previous_temperature = 21.0
    minimum_temperature = 5.0
    maximum_temperature = 30.0
    previous_mode = 'CUSTOM'
    target_mode = 'CUSTOM'
    request_name = request['header']['name']
    message_id = request['header']['messageId']
    
    # valid request    
    if request_name in ['SetTargetTemperatureRequest','IncrementTargetTemperatureRequest','DecrementTargetTemperatureRequest']:
        if request_name == 'SetTargetTemperatureRequest': 
            response_name = 'SetTargetTemperatureConfirmation'
            target_temperature = request['payload']['targetTemperature']['value']
        if request_name == 'IncrementTargetTemperatureRequest':
            response_name = 'IncrementTargetTemperatureConfirmation'
            target_temperature = previous_temperature + request['payload']['deltaTemperature']['value']
        if request_name == 'DecrementTargetTemperatureRequest':
            response_name = 'DecrementTargetTemperatureConfirmation'
            target_temperature = previous_temperature - request['payload']['deltaTemperature']['value']

        payload = {
            'targetTemperature': {
                'value': target_temperature
            },
            'temperatureMode': {
                'value': target_mode
            },
            'previousState' : {
                'targetTemperature':{
                    'value': previous_temperature
                },
                'temperatureMode':{
                    'value': previous_mode
                }
            }        
        }
    
    header = generateResponseHeader(request,response_name)
    response = generateResponse(header,payload)
    return response
    
    
    
def generateLightingResponse(request):
    request_name = request['header']['name']
    message_id = request['header']['messageId']
    payload = {}
    if request_name == 'SetColorRequest':
        response_name = 'SetColorConfirmation'
        payload = {
            'achievedState': {
                'color': {
                    'hue': request['payload']['color']['hue'],
                    'saturation': request['payload']['color']['saturation'],
                    'brightness': request['payload']['color']['brightness'],
                }
            }
        }
        
    header = generateResponseHeader(request,response_name)
    response = generateResponse(header,payload)
    return response
    


def getRGB(request):
    h = float( request['payload']['color']['hue'])
    s = float(request['payload']['color']['saturation'])
    v = float(request['payload']['color']['brightness'])
    h60 = h / 60.0
    h60f = math.floor(h60)
    hi = int(h60f) % 6
    f = h60 - h60f
    p = v * (1 - s)
    q = v * (1 - f * s)
    t = v * (1 - (1 - f) * s)
    r, g, b = 0, 0, 0
    if hi == 0: r, g, b = v, t, p
    elif hi == 1: r, g, b = q, v, p
    elif hi == 2: r, g, b = p, v, t
    elif hi == 3: r, g, b = p, q, v
    elif hi == 4: r, g, b = t, p, v
    elif hi == 5: r, g, b = v, p, q
    r, g, b = int(r * 255), int(g * 255), int(b * 255)
    return '%02x%02x%02x' % (r, g, b)

def getActor(actorID):
    global actors
    for actor in actors:
        if str(actor["id"]) == str(actorID):
            return actor
        
    return


