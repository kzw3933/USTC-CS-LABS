import json
from config import *

class DB(object):
    def __init__(self):
        
        self.users = self.database_init()

    def database_init(self):

        with open(DATA_BASE_FILE,"r") as f:
            users = json.load(f)
            return users

    def get_user(self,username):
        if username in self.users.keys(): 
            return self.users[username]
        else:
            return None




