import mysql.connector

cnx = mysql.connector.connect(user='root', password='root',
                              host='127.0.0.1',
                              database='bot')

cursor = cnx.cursor()


add_quest = ("INSERT INTO quest "
               "(id, giver_id) "
               "VALUES ({}, {})".format(1,1))

cursor.execute(add_quest)
cnx.commit()
cnx.close()