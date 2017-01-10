/*
 * This file is part of umqtt.
 *
 * umqtt is free softare: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Softare Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it ill be useful,
 * but WITHOUT ANY WARRANTY; ithout even the implied arranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License									    along ith Foobar.  If not, see <http://.gnu.org/licenses/
 * */

#ifndef __UMQTT_H__
#define __UMQTT_H__

#include <stdint.h>

enum umqtt_packet_type {
	UMQTT_CONNECT		= 1,
	UMQTT_CONNACK		= 2,
	UMQTT_PUBLISH		= 3,
	UMQTT_SUBSCRIBE		= 8,
	UMQTT_SUBACK		= 9,
	UMQTT_UNSUBSCRIBE	= 10,
	UMQTT_UNSUBACK		= 11,
	UMQTT_PINGREQ		= 12,
	UMQTT_PINGRESP		= 13,
	UMQTT_DISCONNECT	= 14,
};

enum umqtt_client_state {
	UMQTT_STATE_INIT,
	UMQTT_STATE_CONNECTING,
	UMQTT_STATE_CONNECTED,
	UMQTT_STATE_FAILED,
};

struct umqtt_circ_buffer {
	uint8_t *start;
	int length;

	/* Private */
	uint8_t *pointer;
	int datalen;
};

struct umqtt_connection {
	struct umqtt_circ_buffer txbuff;
	struct umqtt_circ_buffer rxbuff;

	void (*message_callback)(struct umqtt_connection *,
			char *topic, uint8_t *data, int len);

	/* Private */
	/* ack counters - incremented on sending, decremented on ack */
	int nack_publish;
	int nack_subscribe;
	int nack_ping;

	int message_id;

	uint8_t work_buf[5];
	int work_read;

	enum umqtt_client_state state;
};

#define umqtt_circ_datalen(buff)  ((buff)->datalen)

#define umqtt_circ_is_full(buff)  ((buff)->length == (buff)->datalen)

#define umqtt_circ_is_empty(buff) (umqtt_circ_datalen(buff) == 0)

void umqtt_circ_init(struct umqtt_circ_buffer *buff);

/* Return the amount of bytes left */
int umqtt_circ_push(struct umqtt_circ_buffer *buff, uint8_t *data, int len);

/* Returns amount of bytes popped/peeked */
int umqtt_circ_pop(struct umqtt_circ_buffer *buff, uint8_t *data, int len);
int umqtt_circ_peek(struct umqtt_circ_buffer *buff, uint8_t *data, int len);

void umqtt_init(struct umqtt_connection *conn);
void umqtt_connect(struct umqtt_connection *conn, uint16_t kalive, char *cid);
void umqtt_subscribe(struct umqtt_connection *conn, char *topic);
void umqtt_publish(struct umqtt_connection *conn, char *topic,
		uint8_t *data, int datalen);
void umqtt_ping(struct umqtt_connection *conn);
void umqtt_process(struct umqtt_connection *conn);

#endif /* __UMQTT_H__ */
