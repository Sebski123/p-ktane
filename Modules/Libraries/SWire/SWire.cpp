/** @file SWire.cpp
 *  @brief The SWire library implementation
 *
 *  @author Sebastian Mason (sebski123)
 */

#include "Arduino.h"
#include "SWire.h"
#include <string.h>
#include "stringQueue.h"

stringQueue_t _in_messages;
char currentCommand;

const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

/** @brief Reads a packet from the specified stream if one is available
 *
 *  If the data in the stream contains a full packet, the packet will be put
 *  into the return buffer and the return code will indicate it'_s validity.
 *  If there is not enough data in the stream buffer for a full packet, the
 *  function not populate the buffer and the return code will indicate no new
 *  packet.
 *
 *  Returned data DOES include the address as the first byte of the buffer.
 *
 *  @param buffer A pointer to the buffer to populate with the possible packet
 *  @return A status code indicating the status of the packet:
 *            0 - No new packet, buffer is returned empty.
 *            1 - New packet in buffer, packet is valid.
 *           -1 - New packet in buffer, packet failed parity check.
 */
int readPacket(char *buffer)
{
  static int in_packet = 0;
  static int index = 0;
  static char buf[MAX_MSG_LEN + 1];
  static char data_parity = 0;
  static int escape_next = 0;
  char rc;
  int passed_parity = 0;

  while (Wire.available() > 0)
  {
    rc = Wire.read();
    if (rc == START)
    {
      in_packet = 1;
      data_parity = START;
      index = 0;
    }
    else if (in_packet == 1)
    {
      data_parity ^= rc;
      if (rc != END)
      {

        buf[index] = rc;
        escape_next = 0;
        index++;
      }
      if (rc == END || index >= MAX_MSG_LEN)
      {
        index--;
        buf[index] = '\0'; //purposefully overwrite parity byte.
        strcpy(buffer, buf);
        passed_parity = ((data_parity & 0x7F) == 0);
        index = 0;
        in_packet = 0;
        data_parity = 0;
        if (passed_parity)
        {
          return 1;
        }
        else
        {
          return -1;
        }
      }
    }
  }
  return 0;
}

/** @brief Writes a packet to the I2C bus.
 * 
 *  @param message  A pointer to the message to send, first byte should be
                      a client address, message should be null terminated,
                      however the null byte will not be sent over the wire.
 *  @return A status code indicating the whether or not the message was sent
 *            - Currently this function will always succeed
 */
int sendPacket(char *message)
{
  char data_parity = 0;

  if (strlen(message) == 0)
  {
    return 0; // Should only happen on library failure
  }
  for (int i = 0; message[i] != 0; i++)
  {
    data_parity = data_parity ^ (uint8_t)message[i];
  }

  Wire.beginTransmission(message[0]);
  Wire.write(START);
  for (int i = 0; message[i] != 0; i++)
  {
    Wire.write(message[i]);
  }
  Wire.write((START ^ data_parity ^ END));
  Wire.write(END);
  Wire.endTransmission();
  return 1;
}

void receiveEvent(int howMany)
{
  char *buffer = (char *)malloc(MAX_MSG_LEN + 1);
  if (buffer == NULL)
  { // Fail if buffer allocation failed
    return;
  }
  int result = readPacket(buffer);
  if (result == -1)
  {
    currentCommand == NAK;
  }
  else if (result == 0)
  {
    currentCommand == NO_DATA;
  }
  else
  {
    currentCommand = buffer[1]; // remember command for when we get request
    stringQueueAdd(&_in_messages, buffer);
  }
} // end of receiveEvent

void requestEvent()
{
  switch (currentCommand)
  {
  case NAK:
    Wire.write(NAK);
    break;
  case NO_DATA:
    break;
  default:
    Wire.write(ACK);
    break;

  } // end of switch*/

} // end of requestEvent

/** @brief Creates a new SWireMaster object
 * 
 *  @param i2c_addr I2C address of SWireMaster device
 *  @return A new initialized SWireMaster object
 */
SWireMaster::SWireMaster(int i2c_addr)
{
  _num_clients = 0;
  memset(_clients, 0, MAX_CLIENTS);
  stringQueueInit(&_in_messages, MAX_MASTER_QUEUE_SIZE);
  Wire.begin(i2c_addr);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

/** @brief sends a data string to the specified client.
 *
 *  Internally, this function enqueues the data to be written when convenient.
 *  This function fails when there is not enough memory to enqueue the message
 *  or when the queue is full.
 *
 *  @param client_id  The ID of the client to write to
 *  @param data       The data to write to the given client
 *  @return A status code indicating success or failure
 */
int SWireMaster::sendData(uint8_t client_id, char *data)
{
  char *new_message = (char *)malloc(MAX_MSG_LEN + 1);
  if (new_message == NULL)
  {
    return 0;
  }
  strcpy(new_message + 1, data);
  new_message[0] = (char)client_id;

  Serial.print("Sending: ");
  Serial.println(new_message);

  sendPacket(new_message);
  if (Wire.requestFrom((int)client_id, 1) != 0)
  {
    return 1;
  }
  return 0;
}

/** @brief Retrieve data if there is any to get
 *
 *  @param buffer A string to populate with the possible data
 *  @return The ID of the client that sent the message, 0 if no data.
 */
int SWireMaster::getData(char *buffer)
{
  int client_id;
  char *message;
  if (stringQueueIsEmpty(&_in_messages))
  {
    return 0;
  }
  Serial.println("queue is not empty");
  message = stringQueueRemove(&_in_messages);
  client_id = message[1];
  strcpy(buffer, message + 2);
  free(message);
  return client_id;
}

/** @brief runs a client search
 *
 *  A client search consists of pinging each client address between 1 and 
 *  MAX_CLIENTS. If the client responds, then it gets put in our array.
 *
 *  @return The number of clients found
 */
int SWireMaster::identifyClients()
{
  //_s = s;
  unsigned long start_millis;
  char temp[MAX_MSG_LEN];
  char message[3] = {(char)1, PING, '\0'};
  _num_clients = 0;
  memset(_clients, 0, MAX_CLIENTS);

  for (int i = 1; i < MAX_CLIENTS; i++)
  {
    message[0] = (char)i;
    sendPacket(message);
    if (Wire.requestFrom(i, 1) != 0)
    {
      Serial.print("Req data is: ");
      Serial.println(Wire.read());
      _clients[_num_clients] = i;
      _num_clients++;
    }
  }
  return _num_clients;
}

/** @brief gets the client array and number of clients
 *
 *  If the clients argument is NULL, the number of clients will still be
 *  returned, but it will not attempt to populate the array.
 *
 *  @param clients  a pointer to memory of at least MAX_CLIENTS size to put
                      the the clients into 
 *  @return The number of clients found
 */
int SWireMaster::getClients(uint8_t *clients)
{
  if (clients != NULL)
  {
    memcpy(clients, _clients, _num_clients);
  }
  return _num_clients;
}

/** @brief Creates a new SWireClient object
 * 
 *  @param port The underlying stream object used for communication.

 *  @return A new initialized SWireClient object
 */
SWireClient::SWireClient(uint8_t client_number)
{
  _client_number = client_number;
  stringQueueInit(&_in_messages, MAX_CLIENT_QUEUE_SIZE);
  Wire.begin(client_number);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

/** @brief sends a data string to the master.
 *
 *  Internally, this function enqueues the data to be written when convenient.
 *  This function fails when there is not enough memory to enqueue the message
 *  or when the queue is full.
 *
 *  @param data The data to write to the given client
 *  @return A status code indicating success or failure
 */
int SWireClient::sendData(char *data)
{
  char *new_message = (char *)malloc(MAX_MSG_LEN + 2);
  if (new_message == NULL)
  {
    return 0;
  }
  strcpy(new_message + 2, data);
  new_message[0] = 1; //Master id
  new_message[1] = (char)_client_number;

  Serial.print("Sending: ");
  Serial.println(new_message);

  sendPacket(new_message);
  if (Wire.requestFrom(1, 1) != 0)
  {
    Serial.print("Req data is: ");
    Serial.println(Wire.read());
    return 1;
  }
  return 0;
}

/** @brief Retrieve data if there is any to get
 *
 *  @param buffer A string to populate with the possible data
 *  @return A status code indicating whether data was retrieved
 */
int SWireClient::getData(char *buffer)
{
  char *message;
  if (stringQueueIsEmpty(&_in_messages))
  {
    return 0;
  }
  message = stringQueueRemove(&_in_messages);
  strcpy(buffer, message + 1);
  free(message);
  return 1;
}
