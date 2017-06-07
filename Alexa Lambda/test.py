import urllib
import json


myactors = urllib.urlopen("https://d0c2baeb.ngrok.io/SHP-Web/rest/gui/thing/actor/a/a").read()
actors = json.loads(myactors)
number = 4



		

	
	
payload = {
	"discoveredAppliances":[]
	}
	
for actor in actors:
	number=number+1
	payload["discoveredAppliances"].append(		{		
				"applianceId":"device003",
				"manufacturerName":"yourManufacturerName",
				"modelName":"model 0 " + str(number),
				"version":"your software version number here.",
				"friendlyName":actor,
				"friendlyDescription":"Virtual Device for the Sample Hello World Skill",
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
			
print payload

