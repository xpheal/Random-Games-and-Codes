import requests
from lxml import etree
from io import StringIO

def debug_response(r):
	print("\nUrl: {}".format(r.url))
	print("Status_code: {}".format(r.status_code))
	print("Cookies: {}".format(r.cookies.get_dict()))
	print("Headers: {}".format(r.headers))

def save_response(r, name):
	with open(name, 'w') as f:
		f.write(r.text)

with requests.Session() as s:
	payload = {
		'j_username' : 'someid',
		'j_password' : 'somepassword',
		'_eventId_proceed': ''
	}
	url = 'https://my.wisc.edu'

	# Login
	r1 = s.get(url)
	r2 = s.post(r1.url, data=payload)

	tree = etree.parse(StringIO(r2.text), etree.HTMLParser())
	url = etree.XPath('/html/body/form/@action')(tree)[0]
	
	payload = {
		'RelayState' : etree.XPath('/html/body/form/div/input[1]/@value')(tree),
		'SAMLResponse' : etree.XPath('/html/body/form/div/input[2]/@value')(tree)
	}

	# Access web app
	r3 = s.post(url, data=payload)

	# Get grades
	url = 'https://my.wisc.edu/portal/f/u360303l1s4/p/FinalGrades.n89/max/render.uP'
	r4 = s.get(url)

# Parse results
tree = etree.parse(StringIO(r4.text), etree.HTMLParser())

subject = etree.XPath('//*[@id="Pluto_396_n89_309520__grades_data"]/li/text()')(tree)
letter_grade = etree.XPath('//*[@id="Pluto_396_n89_309520__grades_data"]/li/div/div[4]/text()')(tree)

subject = [x.strip() for x in subject if x.strip()]
letter_grade = letter_grade[1:]

for s,l in zip(subject, letter_grade):
	print("Subject: {}\nGrade: {}\n\n".format(s, l))
