import paho.mqtt.client as mqtt
import  do_json
import paho.mqtt.publish as publish
import sys
def pub_msg(msg, topic, client_id = "wechat_cmd"):
    hostname = ""
    port = 1883
    if topic == "cmd":
		publish.single("cmd", msg, port= port, hostname=hostname, client_id=client_id)
    	#write in file
    	#do_json.process_json('data_in.json', 'data_out.json', True, msg, True)
    	#do_json.process_json('data_out.json', 'data_in.json')
    if topic == "music":
    	publish.single("music", msg, port= port, hostname=hostname, client_id=client_id)

#msg1 = "{\"ctr_light\":1}"
#msg2 = "{\"ctr_light\":2}"
#msg3 = "{\"ctr_light\":100}"
#pub_msg(msg1,"cmd")
#pub_msg(msg2,"music")
#pub_msg(msg3,"cmd")
