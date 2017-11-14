/**
 * Welcome to Pebble.js!
 *
 * This is where you write your app.
 */

var UI = require('ui');
var Vector2 = require('vector2');
var Light = require('ui/light');

var Accel = require('ui/accel');

var state=0;
var lcd_state=1;
var auto_on=0;

var last_time=0;
var API_URL='http://10.10.10.1/';
if (localStorage.getItem('last_time')){ last_time=parseInt(localStorage.getItem('last_time'));}

var ajax = require('ajax');




var Vibe = require('ui/vibe');

Accel.config({rate:10,samples:5});


function onoff(){
  if (state){
    
    Vibe.vibrate('double');
    ajax({ url: API_URL+'on' });
  }
  else{
    Vibe.vibrate('short');
    ajax({ url: API_URL+'off' });
  }
  
}




Accel.on('data', function(e) {
  if (!auto_on){return;}
  else{
  var t=false;
  if (parseInt(e.accel.x/400)<0){t=true;}
  else{t=false;}
  
  if (t!=state){state=t;onoff();}
    
  }
  
});





var splash = new UI.Window({
  backgroundColor:'#aaaaaa',
  action: {
    down: 'images/hh.png',
    up: 'images/lighticon.png',
    select:'images/stopwatch.png'
  }

});
splash.status({
  color: 'white',
  backgroundColor: 'black',
  separator:'none'
});




var text_main = new UI.Text({
  position: new Vector2(5, 44),
  size: new Vector2(120, 50),
  color:'black',
  font: 'leco-36-bold-numbers',
  textAlign:'left',
  textOverflow:'fill',
  text:'00:00'
  });

splash.add(text_main);

splash.show();
splash.on('click', 'up', function() {
  auto_on=false;
  state=!state;
  onoff();
});


splash.on('click', 'down', function() {
  auto_on=!auto_on;
  if (!auto_on){state=false;}
  splash.action('down','');
  if (auto_on){splash.action('down','images/handoff.png');}
  else{splash.action('down','images/hh.png');}
});
splash.on('longClick', 'select', function() {
  last_time=0;
});

function exit()
{
    localStorage.setItem('last_time',last_time);
}

splash.on('hide', exit);

function lcd(){
  if (state){
    if (lcd_state){Light.on();}
    else{Light.auto();}
    lcd_state=!lcd_state;
  }
  else{
    Light.on();
  }
  
}

function secs_upd(){
  if (state){
    last_time+=1;
    
  }
  text_main.text(new Date(last_time * 1000).toISOString().substr(14, 5));
  
}


setInterval(lcd,200);
setInterval(secs_upd,1000);