import urllib
import json

def lambda_handler(event, context):
    access_token = event['payload']['accessToken']

    if event['header']['namespace'] == 'Alexa.ConnectedHome.Discovery':
        return handleDiscovery(context, event)

    elif event['header']['namespace'] == 'Alexa.ConnectedHome.Control':
        return handleControl(context, event)

def handleDiscovery(context, event):
    #mydata = urllib.urlopen("https://d0c2baeb.ngrok.io/SHP-Web/alexa.html").read()
    payload = ''
    header = {
        "namespace": "Alexa.ConnectedHome.Discovery",
        "name": "DiscoverAppliancesResponse",
        "payloadVersion": "2"
        }

    if event['header']['name'] == 'DiscoverAppliancesRequest':
    	myactors = urllib.urlopen("https://d0c2baeb.ngrok.io/SHP-Web/rest/gui/thing/actor/a/a").read()
    	actors = json.loads(myactors)
    	number = 9
    	payload = {
    		"discoveredAppliances":[]
    		}

    	for actor in actors:
    		number = (int(str(number))) + 1
    		payload["discoveredAppliances"].append(		{		
    					"applianceId":"device00" + str(number),
    					"manufacturerName":"yourManufacturerName",
    					"modelName":"model 0" + str(number),
    					"version":"your software version number here.",
    					"friendlyName":actor,
    					"friendlyDescription":"Mein Super Cooles Lambda Geraet",
    					"isReachable":True,
    					"actions":[
    						"turnOn",
    						"turnOff"
    					],
    					"additionalApplianceDetails":{
    						"extraDetail1":"optionalDetailForSkillAdapterToReferenceThisDevice",
    						"extraDetail2":"There can be multiple entries",
    						"extraDetail3":"but they should only be used for reference purposes.",
    						"extraDetail4":"This is not a suitable place to maintain current device state"
    					}
    				})

    return { 'header': header, 'payload': payload }

def handleControl(context, event):
    payload = ''
    device_id = event['payload']['appliance']['applianceId']
    message_id = event['header']['messageId']

    if event['header']['name'] == 'TurnOnRequest':
        
        payload = { }

    header = {
        "namespace":"Alexa.ConnectedHome.Control",
        "name":"TurnOnConfirmation",
        "payloadVersion":"2",
        "messageId": message_id
        }
    return { 'header': header, 'payload': payload }

