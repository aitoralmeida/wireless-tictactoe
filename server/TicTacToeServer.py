# -*- coding: utf-8 -*-
"""
Created on Fri Jul 04 12:10:33 2014

@author: aitor
"""

from socket import *
import thread

HOST, PORT = "localhost", 9999

END_CHAR = '\0d'
CMD_REGISTER = "REGISTER"
CMD_FIND_GAME = "FIND_GAME"
MSG_WAITING_PLAYERS = "WAITING_FOR_PLAYERS"
MSG_RIVAL = "RIVAL"
MSG_UNREGISTERED = "UNREGISTERED"



BUFF = 1024
HOST = '127.0.0.1'
PORT = 9999 

class TicTacToeServer:
    
    def __init__(self):
        self.waiting_players = []
        self.player1 = ''
        self.player2 = ''

    def response(key):
        return 'Server response: ' + key
    
    def handler(self, clientsock,addr):
        data = clientsock.recv(BUFF)
        if data: 
            print repr(addr) + ' recv:' + repr(data)
            #clientsock.send(response(data))
            #print repr(addr) + ' sent:' + repr(response(data))
                        
            if data.startswith(CMD_REGISTER):
                player_id = data.split(' ')[1]
                self.waiting_players.append(player_id)
                print "REGISTER %s" % (player_id)
                print "Total players: %i" % (len(self.waiting_players))
            elif data.startswith(CMD_FIND_GAME):
                print data.split(' ')
                player_id = data.split(' ')[1]
                print "FIND_GAME %s" % (player_id)

                if self.player2 == player_id: # When doing it randomly it will not be player2
                    print "Turn already assigned."
                    clientsock.send("%s %s %i%s" % (MSG_RIVAL, self.player1, 1, END_CHAR))
                    
                elif self.player1 == player_id: # When doing it randomly it will not be player2
                    print "Turn already assigned."
                    clientsock.send("%s %s %i%s" % (MSG_RIVAL, self.player2, 0, END_CHAR))
                    
                elif player_id not in self.waiting_players:
                    print 'Player has to register first.'
                    clientsock.send(MSG_UNREGISTERED + END_CHAR)
                    
                
                elif len(self.waiting_players) > 1: 
                    print "Assigning turn."
                    #TODO do this randomly
                    self.player1 = player_id
                    # Find the second player
                    for player in self.waiting_players:
                        if player != player_id:
                            self.player2 = player
                            break
                        
                    # Remove assigned players
                    self.waiting_players.remove(self.player1)
                    self.waiting_players.remove(self.player2)
                    clientsock.send("%s %s %i%s" % (MSG_RIVAL, self.player2, 0, END_CHAR))                                     
                else:
                    print "Wait for players."
                    clientsock.send(MSG_WAITING_PLAYERS + END_CHAR)

    
        clientsock.close()
        print addr, "- closed connection" #log on console
        
    def start(self):
        ADDR = (HOST, PORT)
        serversock = socket(AF_INET, SOCK_STREAM)
        serversock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
        serversock.bind(ADDR)
        serversock.listen(5)
        while True:
            print 'Listening on port', PORT
            clientsock, addr = serversock.accept()
            print 'New connections:', addr
            thread.start_new_thread(self.handler, (clientsock, addr))

if __name__=='__main__':
    server = TicTacToeServer()
    server.start()

    
    
    
    
    
    
    
    
    
#    
#    
#    
#    
#    
#if self.data.startswith("REGISTER"):
#            player_id = self.data.split(' ')[1]
#            player_ip = self.client_address[0]
#            self.waiting_players.append((player_id, player_ip))
#            print "REGISTER %s" % (player_id)
#            print "Total players: %i" % (len(self.waiting_players))
#        elif self.data.startswith("FIND_GAME"):
#            player_id = self.data.split(' ')[1]
#            player_ip = self.client_address[0]
#            print "FIND_GAME %s" % (player_id)
#            
#            if self.player2[0] == player_id:
#               self.request.sendall("%s %s%s" % (MSG_RIVAL, self.player1[0], END_CHAR)) 
#            
#            elif len(self.waiting_players) > 2:                
#                turn = random.random()
#                if turn < 0.50:
#                    self.player1 = (player_id, player_ip)
#                    # Find the second player
#                    for player in self.waiting_players:
#                        if player[0] != player_id:
#                            self.player2 = player
#                            break
#                        
#                    # Remove assigned players
#                    self.waiting_players.remove(self.player1)
#                    self.waiting_players.remove(self.player2)
#                    self.request.sendall("%s %s%s" % (MSG_RIVAL, self.player2[0], END_CHAR)) 
#                                    
#            else:
#                self.request.sendall(MSG_WAITING_PLAYERS + END_CHAR)