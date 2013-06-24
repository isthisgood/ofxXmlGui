import BaseHTTPServer
import cgi, random, sys
import SimpleHTTPServer
import os

PORT = 8000
httpd = BaseHTTPServer.HTTPServer(("", PORT), SimpleHTTPServer.SimpleHTTPRequestHandler)
print "Serving at port", PORT


httpd.serve_forever()
