### source: https://github.com/alexa/alexa-smarthome-validation/edit/master/python/lambda.py ###
import urllib
import json

ngrok = "https://ffdecb84.ngrok.io"

def lambda_handler(event, context):
    access_token = event['payload']['accessToken']

    if event['header']['namespace'] == 'Alexa.ConnectedHome.Discovery':
        return handleDiscovery(context, event)

    elif event['header']['namespace'] == 'Alexa.ConnectedHome.Control':
        return handleControl(context, event)

def handleDiscovery(context, event):
    global ngrok
    #mydata = urllib.urlopen("https://d0c2baeb.ngrok.io/SHP-Web/alexa.html").read()
    payload = ''
    header = {
        "namespace": "Alexa.ConnectedHome.Discovery",
        "name": "DiscoverAppliancesResponse",
        "payloadVersion": "2"
        }

    if event['header']['name'] == 'DiscoverAppliancesRequest':
    	myactors = urllib.urlopen(ngrok + "/SHP-Web/rest/gui/thing/actor/a/a").read()
    	actors = json.loads(myactors)
    	number = 9
    	payload = {
    		"discoveredAppliances":[]
    		}

    	for actor in actors:
    		number = (int(str(number))) + 1
    		payload["discoveredAppliances"].append(		{		
    					"applianceId":str(actor["id"]),
    					"manufacturerName":"yourManufacturerName",
    					"modelName":str(actor["id"]),
    					"version":"your software version number here.",
    					"friendlyName":str(actor["name"]),
    					"friendlyDescription":"Mein Super Cooles Lambda Geraet",
    					"isReachable":True,
    					"actions":[
    						"turnOn",
    						"turnOff",
    						"setTargetTemperature"
    					],
    					"additionalApplianceDetails":{
    						"extraDetail1":"optionalDetailForSkillAdapterToReferenceThisDevice",
    						"extraDetail2":"There can be multiple entries",
    						"extraDetail3":"but they should only be used for reference purposes.",
    						"extraDetail4":"This is not a suitable place to maintain current device state"
    					}
    				})

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
    
def handleControl(context, event):
    global ngrok
    payload = ''
    device_id = event['payload']['appliance']['applianceId']
    message_id = event['header']['messageId']
    previous_temperature = 21.0
    minimum_temperature = 5.0
    maximum_temperature = 30.0

    if event['header']['name'] == 'TurnOnRequest':
        urllib.urlopen(ngrok + "/SHP-Web/rest/gui/thing/" + str(device_id) + "/1/a/a"  ).read()
        payload = { }
        header = {
            "namespace":"Alexa.ConnectedHome.Control",
            "name":"TurnOnConfirmation",
            "payloadVersion":"2",
            "messageId": message_id
            }
    
    if event['header']['name'] == 'TurnOffRequest':
        urllib.urlopen(ngrok + "/SHP-Web/rest/gui/thing/" + str(device_id) + "/0/a/a"  ).read()
        payload = {}
        header = {
            "namespace":"Alexa.ConnectedHome.Control",
            "name":"TurnOffConfirmation",
            "payloadVersion":"2",
            "messageId": message_id
            }
    if event['header']['name'] == 'SetTargetTemperatureRequest': 
        target_temperature = event['payload']['targetTemperature']['value']
        urllib.urlopen(ngrok + "/SHP-Web/rest/gui/thing/" + str(device_id) + "/" + str(target_temperature) +"/a/a"  ).read()
        #urllib.urlopen(ngrok + "/SHP-Web/rest/gui"  ).read() 
        previous_mode = 'CUSTOM'
        target_mode = 'CUSTOM'
        response = generateTemperatureResponse(event,previous_temperature,previous_mode,target_mode,minimum_temperature,maximum_temperature)
        return response
        #payload = {}
        #header = {
        #    "namespace":"Alexa.ConnectedHome.Control",
        #    "name":"SetTargetTemperatureConfirmation",
        #    "payloadVersion":"2",
        #    "messageId": message_id
        #    }
        #urllib.urlopen(ngrok + "/SHP-Web/rest/test"  ).read() 
        

    
    return { 'header': header, 'payload': payload }

def generateTemperatureResponse(request,previous_temperature,previous_mode,target_mode,minimum_temperature,maximum_temperature):
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
