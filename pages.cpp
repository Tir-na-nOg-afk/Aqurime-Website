#include <Arduino.h>
#include <WebServer.h>
#include "pages.h"

extern WebServer server;

const char baseCss[] = R"rawliteral(
  body{
    margin:0;
    font-family:Arial;
    backgrond:black;
    color:white
  }
  .navbar{
    background:#111;
    overflow:hidden;
    padding:10px
  }
  .navbar a{
    float:left;
    color:white;
    text-decoration:none;
    padding:14px 20px;
    font-size:18px
  }
  .navbar a:hover{
    background:#333;
    border-radius:5px
  }
  .navbar a.active{
    background:#333;
    border-radius:5px
  }
  .content{
    padding:40px;
    text-align:center;
    max-width:1100;
    margin:0 auto
  }
  h1{
    font-size:50px;
    margin:0 0 20px
  }
  h3{
    margin:0 0 10px
  }
  .card{
    background:#111;
    border-radius:8px;
    padding:16px;
    margin: 12px 0;
    text-align:left
  }
  @media (max-width:600px){
    .content{
      padding:16px
    }
    h1{
      font-size:36px
    }
    .navbar a{
      padding:10px 12px;
      font-size:16px
    }
  }
 )rawliteral";

String pageHead(String title, String extraCSS){
  String h = "<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'>";
  h += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  h +="<title>" + title + "</title><link rel='icon' href='data:,'>";
  h += "<style>" + String(baseCss) + extraCSS + "</style></head><body>";
  return h;
}

String navBar(bool big, int active) {
  const char* href[4] = {"/","/lights","/motors","/sensors"};
  const char* label [4] = {"Home", "Lights","Motors", "Sensors"};
  String s = big? "<nav class='navbar big'>" : "<nav class='navbar'>";
  if (big) s += "<span class='brand'>Aquarium Controller</span>";
  for (int i = 0; i< 4; i++){
    s += "<a href='" + String(href[i]) + "'";
    if (i== active) s += " class='active'";
    s += ">" + String(label[i]) + "</a>";
  }
  return s + "</nav>";
}

String pageEnd() {
  return "</body></html>";
}

const char homeCss[] = R"rawliteral(
  .navbar.big{
    position:sticky;
    top:0;
    padding:16px 24px;
    background:linear-gradient(90deg,#0d1b2a,#1b4332);
    box-shadow:0 2px 10px #000
  }
  .navbar.big .brand{
    float:left;
    font-size:26px;
    font-weight:bold;
    color:#9be3b8;
    padding:10px 28px 10px 0
  }
  .navbar.big a{
    font-size:22px;
    border-radius:8px;
    margin-right:6px
  }
  .navbar.big a:hover,.navbar.big a.active{
    background:#2d6a4f
  }
  .tiles{
    display:grid;
    grid-template-columns:repeat(auto-fit,minmax(200px,1fr));
    gap:16px;
    margin-top:30px
  }
  .tile{
    display:block;background:#111;border-radius:12px;padding:28px 10px;color:white;text-decoration:none;font-size:22px
  }
  .tile:hover{
    background:#1b4332
  }
  @media (max-width:700px){
    .navbar.big{
      padding:8px
      }
    .navbar.big .brand{
      display:none
      }
    .navbar.big a{
      font-size:17px;
      padding:10px 12px;
      margin-right:2px
    }
  }
)rawliteral";

const char homeBody[] = R"rawliteral(
<div class='content'>
  <h1>Hello</h1>
  <p>Aquarium Controller</p>
  <div class='tiles'>
    <a class='tile' href='/motors'>Motors</a>
    <a class='tile' href='/sensors'>Sensors</a>
    <a class='tile' href='/lights'>Lights</a>
  </div>
</div>
)rawliteral";
void home() {
  String page = pageHead("Home", homeCss) + navBar(true, 0); 
  page += homeBody;
  page += pageEnd();
  server.send(200, "text/html", page);
}

void lights() {
  server.send(200, "text/html", pageHead("Lights", "") + navBar(false, 1) + "<div class='content'><h1>Lights</h1><p>Coming soon</p></div>" + pageEnd());
}
void motors() { 
  server.send(200, "text/html", pageHead("Motors", "") + navBar(false, 2) + "<div class='content'><h1>Motors</h1></div>" + pageEnd()); 
}
void sensors() { 
  server.send(200, "text/html", pageHead("Sensors", "") + navBar(false, 3) + "<div class='content'><h1>Sensors</h1></div>" + pageEnd()); 
}