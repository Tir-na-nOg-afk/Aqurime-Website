#include <Arduino.h>
#include <WebServer.h>
#include "pages.h"

extern WebServer server;

const char baseCss[] = R"rawliteral(
  body{
    margin:0;
    font-family:'Trebuchet MS',Tahoma,Verdana,sans-serif;
    background:#eaf4f8;
    color:#14364a
  }
  .navbar{
    background:#0b6e99;
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
    background:#ffffff;
    color:#0b6e99;
    border-radius:20px
  }
  .navbar a.active{
    background:#ffffff;
    color:#0b6e99;
    border-radius:20px
  }
  .content{
    padding:40px;
    text-align:center;
    max-width:1100px;
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
    background:#ffffff;
    border:1px solid #c5dde8;
    border-radius:16px;
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

void sensors() { 
  server.send(200, "text/html", pageHead("Sensors", "") + navBar(false, 3) + "<div class='content'><h1>Sensors</h1></div>" + pageEnd()); 
}

const char homeCss[] = R"rawliteral(
  .navbar.big{
    position:sticky;
    top:0;
    padding:16px 24px;
    background:#0b6e99
  }
  .navbar.big .brand{
    float:left;
    font-size:26px;
    font-weight:bold;
    color:#ffffff;
    padding:10px 28px 10px 0
  }
  .navbar.big a{
    font-size:22px;
    border-radius:8px;
    margin-right:6px
  }
  .navbar.big a:hover,.navbar.big a.active{
    background:#ffffff;
    color:#0b6e99;
    border-radius:20px
  }
  .tiles{
    display:grid;
    grid-template-columns:repeat(auto-fit,minmax(200px,1fr));
    gap:16px;
    margin-top:30px
  }
  .tile{
    display:block;
    background:#ffffff;
    border:1px solid #c5dde8;
    border-radius:16px;
    padding:28px 10px;
    color:#14364a;
    text-decoration:none;
    font-size:22px
  }
  .tile:hover{
    background:#d8eef7
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

const char motorsCss[] = R"rawliteral(
.phases{
  display:flex;
  gap:8px;
  margin-bottom:10px
}
.phaseBox{
  flex:1;
  padding:14px 4px;
  background:#d8e8ef;
  color:#14364a;
  border:none;
  border-radius:20px;
  font-size:16px;
  font-family:inherit;
  cursor:pointer
}
.phaseBox.on{
  background:#0b6e99;
  color:#ffffff
}
progress{
  width:100%;
  height:10px
}
input,progress,meter{
  accent-color:#0b6e99
}
svg{
  width:100%;
  height:auto;
  display:block
}
.band{
  fill:#f3f9fb
}
.band.on{
  fill:#d4ebf5
}
.axis{
  stroke:#9bb5c2;
  stroke-width:1
}
.curve{
  fill:none;
  stroke-width:3
}
.p1{
  stroke:#0b6e99
}
.p2{
  stroke:#f08a24
}
.marker{
  stroke:#d62839;
  stroke-width:2;
  stroke-dasharray:6 4
}
svg text{
  fill:#4a6a7a;
  font-size:12px
}
svg .lg1{
  fill:#0b6e99
}
svg .lg2{
  fill:#f08a24
}
.row{
  display:flex;
  align-items:center;
  gap:10px;
  margin:8px 0
}
.row label{
  width:100px
}
.row input[type=range],.row meter{flex:1}
.row output{
  width:80px;
  text-align:right
}
summary{
  cursor:pointer;
  font-size:18px;
  margin-bottom:8px
}
@media (max-width:600px){
  svg text{
    font-size:20px
  }
}
)rawliteral";

const char motorsBody[] = R"rawliteral(
<div class='content'>
  <h1>Motors</h1>

  <div class='card'>
    <h3>Phase</h3>
    <div class='phases'>
      <button class='phaseBox' id='ph0' onclick='setPhase(0)'>Flood tide</button>
      <button class='phaseBox' id='ph1' onclick='setPhase(1)'>High slack</button>
      <button class='phaseBox' id='ph2' onclick='setPhase(2)'>Ebb tide</button>
      <button class='phaseBox' id='ph3' onclick='setPhase(3)'>Low slack</button>
    </div>
    <progress id='prog' value='0' max='1'></progress>
    <small id='left'></small>
    </div>

  <div class='card'>
    <h3>Pump power over one cycle</h3>
    <svg viewBox='0 0 640 310'>
      <rect id='band0' class='band' x='50' y='10' width='145' height='260'/>
      <rect id='band1' class='band' x='195' y='10' width='145' height='260'/>
      <rect id='band2' class='band' x='340' y='10' width='145' height='260'/>
      <rect id='band3' class='band' x='485' y='10' width='145' height='260'/>
      <text id='bt0' x='122' y='24' text-anchor='middle'>Flood</text>
      <text id='bt1' x='267' y='24' text-anchor='middle'>High</text>
      <text id='bt2' x='412' y='24' text-anchor='middle'>Ebb</text>
      <text id='bt3' x='557' y='24' text-anchor='middle'>Low</text>
      <line class='axis' x1='50' y1='10' x2='50' y2='270'/>
      <line class='axis' x1='50' y1='270' x2='630' y2='270'/>
      <text id='yTop' x='46' y='18' text-anchor='end'>100</text>
      <text id='yMid' x='46' y='143' text-anchor='end'>50</text>
      <text x='46' y='274' text-anchor='end'>0</text>
      <polyline id='c1' class='curve p1' points=''/>
      <polyline id='c2' class='curve p2' points=''/>
      <line id='marker' class='marker' x1='50' y1='10' x2='50' y2='270'/>
      <text class='lg1' x='60' y='300'>Pump 1</text>
      <text class='lg2' x='130' y='300'>Pump 2</text>
      <text id='cycleLbl' x='630' y='300' text-anchor='end'></text>
    </svg>
  </div>

  <div class='card'>
    <h3>Live strength</h3>
    <div class='row'><label>Pump 1</label><meter id='m1' min='0' max='100' value='0'></meter><output id='v1'>0</output></div>
    <div class='row'><label>Pump 2</label><meter id='m2' min='0' max='100' value='0'></meter><output id='v2'>0</output></div>
  </div>

  <div class='card'>
    <details>
      <summary>Period (phase lengths)</summary>
      <div class='row'><label for='len0'>Flood tide</label><input type='range' id='len0' min='0' max='21600' step='900' value='0' oninput='showLen(0)' onchange='sendLen(0)'><output id='out0'></output></div>
      <div class='row'><label for='len1'>High slack</label><input type='range' id='len1' min='0' max='21600' step='900' value='0' oninput='showLen(1)' onchange='sendLen(1)'><output id='out1'></output></div>
      <div class='row'><label for='len2'>Ebb tide</label><input type='range' id='len2' min='0' max='21600' step='900' value='0' oninput='showLen(2)' onchange='sendLen(2)'><output id='out2'></output></div>
      <div class='row'><label for='len3'>Low slack</label><input type='range' id='len3' min='0' max='21600' step='900' value='0' oninput='showLen(3)' onchange='sendLen(3)'><output id='out3'></output></div>
    </details>
    <div class='row'><label for='maxStrength'>Max strength</label><input type='number' id='maxStrength' min='0' max='100' value='100' onchange='sendMax()'></div>
  </div>
</div>
)rawliteral";

const char motorsJs[] = R"rawliteral(
const X0 = 50, X1 = 630, YT = 10, YB = 270;
const NAMES = ['Flood tide', 'High slack', 'Ebb tide', 'Low slack'];
let d = {phase: 0, t: 0, p1: 0, p2: 0, max: 100, times: [5, 5, 5, 5]};
const $ = id => document.getElementById(id);
const sum = a => a.reduce((x, y) => x + y, 0);
const get = url => fetch(url, {cache: 'no-store'});
let busy = false;
function fmt(s) {
  if (s < 60) return Math.round(s) + ' s';
  const m = Math.round(s / 60);
  return m < 60 ? m + ' min' : Math.floor(m / 60) + ' h ' + (m % 60) + ' min';
}
function sx(sec, total) { 
  return X0 + sec / total * (X1 - X0); 
}
function sy(v) { 
  return YB - v / (d.max || 1) * (YB - YT); 
}
function curve(ph) {
  const total = sum(d.times), T = d.times[ph], start = sum(d.times.slice(0, ph));
  const pts = [[0, 0]];
  for (let i = 0; i <= 40; i++) {
    const s = T * i / 40;
    pts.push([start + s, d.max * Math.sin(Math.PI * s / T)]);
  }
  pts.push([total, 0]);
  return pts.map(p => sx(p[0], total).toFixed(1) + ',' + sy(p[1]).toFixed(1)).join(' ');
}
function draw() { 
  const total = sum(d.times);
  for (let i = 0; i < 4; i++) {
    const start = sum(d.times.slice(0, i)), w = d.times[i] / total * (X1 - X0);
    $('band' + i).setAttribute('x', sx(start, total));
    $('band' + i).setAttribute('width', w);
    $('band' + i).classList.toggle('on', i === d.phase);
    $('bt' + i).setAttribute('x', sx(start, total) + w / 2);
    $('bt' + i).style.display = w < 40 ? 'none' : '';
  }
  $('c1').setAttribute('points', curve(0));
  $('c2').setAttribute('points', curve(2));
  const mx = sx(sum(d.times.slice(0, d.phase)) + d.t, total);
  $('marker').setAttribute('x1', mx);
  $('marker').setAttribute('x2', mx);
  $('yTop').textContent = d.max;
  $('yMid').textContent = d.max / 2;
  $('cycleLbl').textContent = 'cycle ' + fmt(total);
}
function showStatus() {
  for (let i = 0; i < 4; i++) $('ph' + i).classList.toggle('on', i === d.phase);
  $('prog').max = d.times[d.phase];
  $('prog').value = d.t;
  $('left').textContent = ' ' + fmt(Math.round(d.times[d.phase] - d.t)) + ' left in ' + NAMES[d.phase];
  $('m1').max = d.max || 1;
  $('m2').max = d.max || 1;
  $('m1').value = d.p1;
  $('m2').value = d.p2;
  $('v1').textContent = Math.round(d.p1);
  $('v2').textContent = Math.round(d.p2);
}
function syncInputs() {
  for (let i = 0; i < 4; i++) {
    if (document.activeElement !== $('len' + i)) {
      $('len' + i).value = d.times[i];
      $('out' + i).textContent = fmt(d.times[i]);
    }
  }
  if (document.activeElement !== $('maxStrength')) $('maxStrength').value = d.max;
}
async function refresh() { 
  if (busy) return;
  busy = true;
  try {
    d = await (await get('/api/motor')).json();
  } 
  catch (e) {
    busy = false;
    return;
  }
  busy = false;
  showStatus();
  syncInputs();
  draw();
}
async function setPhase(p) {
  await get('/api/setPhase?p=' + p);
  refresh();
}
function showLen(i) { 
  $('out' + i).textContent = fmt(+$('len' + i).value); 
}
async function sendLen(i) {
  await get('/api/setPhaseTime?p=' + i + '&s=' + $('len' + i).value);
  refresh();
}
async function sendMax() {
  const v = Math.min(100, Math.max(0, +$('maxStrength').value || 0));
  $('maxStrength').value = v;
  await get('/api/maxStrength?v=' + v);
  refresh();
}
refresh();
setInterval(refresh, 500); 
)rawliteral";

void motors(){
  String page = pageHead("Motors", motorsCss) + navBar(false, 2);
  page += motorsBody;
  page += "<script>";
  page += motorsJs;
  page += "</script>";
  page += pageEnd();
  server.send(200, "text/html", page);
}
