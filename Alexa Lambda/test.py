import urllib
import json

#"https://d0c2baeb.ngrok.io/SHP-Web/rest/gui/thing/actor/a/a").read()
myactors = urllib.urlopen("http://127.0.0.1:8080/SHP-Web/rest/gui/thing/actor/a/a").read()	
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
				"modelName":str(actor["id"]),
				"version":"your software version number here.",
				"friendlyName":str(actor["name"]),
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

