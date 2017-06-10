import urllib2
import urllib
import json

def login():
	url="http://127.0.0.1:8080/SHP-Web/rest/gui/login/a/a"
	request = urllib2.Request(url)
	sock=urllib2.urlopen(request)
	cookies=sock.info()['Set-Cookie']
	content=sock.read()
	sock.close()
	return cookies
#print (cookies, content)

def getURL(url, cookies):
	request = urllib2.Request(url)
	request.add_header("Cookie", cookies)
	sock=urllib2.urlopen(request)
	content=sock.read()
	sock.close()
	return content

def listThings(cookies):
	
	things = json.loads(getURL("http://127.0.0.1:8080/SHP-Web/rest/gui/thing",cookies))
	print "things:"
	for thing in things:
		print thing["name"]
		
	return things;


def listAutomations(cookies):
	autos = json.loads(getURL("http://127.0.0.1:8080/SHP-Web/rest/gui/automation",cookies))
	print "automations:"
	for auto in autos:
		print "automation " + auto["name"] + "\n conditions:"
		for condition in auto["conditions"]:
			print "  " + str(condition["id"])
		print "\n actions:"
		for action in auto["actions"]:
			print "  " + str(action["name"])
		print "\n\n"
	return autos;
	
	
	
cookies = login()
things = listThings(cookies)
autos = listAutomations(cookies)

	
