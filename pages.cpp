#include "pages.h"
#include "aquaServer.h"

client.println("")

void home();
  WifiClient client = server.client();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();

    client.println("<!DOCTYPE html>");
    client.println("<html>");

    client.println("<head><body>");
    client.println("<h1>Home</h1>")
  }
void lights();
    client.println("<head><body>");
    client.println("<h1>Home</h1>")

void motors();

void sensors();

