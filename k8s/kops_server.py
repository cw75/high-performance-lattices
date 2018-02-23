#!/usr/bin/env python3

from http.server import HTTPServer, BaseHTTPRequestHandler
import os
import logging

logging.basicConfig(filename='log.txt',level=logging.INFO)

class KopsHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        if '/add/memory' in self.path:
            num = list(filter(lambda a: a != '', self.path.split('/')))[-1]
            print('Adding ' + num + 'new memory node...')
            logging.info('Adding ' + num + ' new memory node...')
            if os.system('./kops_add_node.sh m ' + num) == 0:
                self.send_response(200)
                self.wfile.write(bytes('Successfully added ' + num + ' memory node.', 'utf-8'))
            else:
                self.send_response(500)
                self.wfile.write(bytes('Unexpected error while adding nodes.', 'utf-8'))
        elif '/add/ebs' in self.path:
            num = list(filter(lambda a: a != '', self.path.split('/')))[-1]
            print('Adding ' + num + 'new EBS node...')
            logging.info('Adding ' + num + ' new EBS node...')
            if os.system('./kops_add_node.sh e ' + num) == 0:
                self.send_response(200)
                self.wfile.write(bytes('Successfully added ' + num + ' EBS node.', 'utf-8'))
            else:
                self.send_response(500)
                self.wfile.write(bytes('Unexpected error while adding nodes.', 'utf-8'))
        elif '/remove/ebs' in self.path:
            print('Removing an EBS node...')
            logging.info('Removing an EBS node...')
            nid = list(filter(lambda a: a != '', self.path.split('/')))[-1]
            if os.system('./remove_node.sh e ' + nid) == 0:
                self.send_response(200)
                self.wfile.write(bytes('Successfully removed an EBS node.', 'utf-8'))
            else:
                self.send_response(500)
                self.wfile.write(bytes('Unexpected error while removing a node.', 'utf-8'))
        elif '/remove/memory' in self.path:
            print('Removing a memory node...')
            logging.info('Removing a memory node...')
            nid = list(filter(lambda a: a != '', self.path.split('/')))[-1]
            if os.system('./remove_node.sh m ' + nid) == 0:
                self.send_response(200)
                self.wfile.write(bytes('Successfully removed a memory node.', 'utf-8'))
            else:
                self.send_response(500)
                self.wfile.write(bytes('Unexpected error while removing a node.', 'utf-8'))
        else:
            self.send_response(404)
            self.wfile.write(bytes('Invalid path: ' + self.path, 'utf-8'))

def run():
    print('starting server...')

    server_address = ('', 80)
    httpd = HTTPServer(server_address, KopsHandler)
    print('running server...')
    httpd.serve_forever()


run()
