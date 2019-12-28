var obniz = new Obniz("9123-9964");
obniz.onconnect = async function () {
  var servo1 = obniz.wired("ServoMotor", {gnd:0, vcc:1, signal:2});
  var servo2 = obniz.wired("ServoMotor", {gnd:0, vcc:1, signal:3});
  var servo3 = obniz.wired("ServoMotor", {gnd:0, vcc:1, signal:4});
  var servo4 = obniz.wired("ServoMotor", {gnd:0, vcc:1, signal:5});

  $("#walk1").on('touchstart mousedown', function(){
	walk_forward(servo1, servo2, servo3, servo4);
  })
  $("#walk1").on('touchend mouseup', function(){
	stop(servo1, servo2, servo3, servo4);
  })

  $("#walk2").on('touchstart mousedown', function(){
	walk_forward(servo1, servo2, servo3, servo4);
  })
  $("#walk2").on('touchend mouseup', function(){
	stop(servo1, servo2, servo3, servo4);
  })

  $("#dance").on('touchstart mousedown', function(){
	dance(servo1, servo2, servo3, servo4);
  })
  $("#dance").on('touchend mouseup', function(){
	stop(servo1, servo2, servo3, servo4);
  })

  $("#stop").on('touchstart mousedown', function(){
	stop(servo1, servo2, servo3, servo4);
  })
};

function walk_forward(servo1, servo2, servo3, servo4){
	loopSleep(8, 800, function(i){
		if(i % 3 == 0) {
			walk_step1(servo1, servo2, servo3, servo4);
		} else if(i % 3 == 1){
			walk_step2(servo1, servo2, servo3, servo4);
		} else {
			walk_step3(servo1, servo2, servo3, servo4);
		}
	});
}

function walk_step1(servo1, servo2, servo3, servo4) {
	servo1.angle(125);
	servo2.angle(125);
	servo3.angle(45);
	servo4.angle(90);
}

function walk_step2(servo1, servo2, servo3, servo4) {
	servo1.angle(90);
	servo2.angle(90);
	servo3.angle(90);
	servo4.angle(90);
}

function walk_step3(servo1, servo2, servo3, servo4) {
	servo1.angle(125);
	servo2.angle(125);
	servo3.angle(90);
	servo4.angle(45);
}

function dance(servo1, servo2, servo3, servo4){
	loopSleep(14, 500, function(i){
		if(i % 2 == 0) {
			dance_step1(servo1, servo2, servo3, servo4);
		} else {
			dance_step2(servo1, servo2, servo3, servo4);
		}
	});
}

function dance_step1(servo1, servo2, servo3, servo4) {
	servo1.angle(90);
	servo2.angle(90);
	servo3.angle(150);
	servo4.angle(30);
}

function dance_step2(servo1, servo2, servo3, servo4) {
	servo1.angle(90);
	servo2.angle(90);
	servo3.angle(30);
	servo4.angle(150);
}

function stop(servo1, servo2, servo3, servo4) {
	servo1.angle(90);
	servo2.angle(90);
	servo3.angle(90);
	servo4.angle(90);
}

function loopSleep(_loopLimit,_interval, _mainFunc){
	var loopLimit = _loopLimit;
	var interval = _interval;
	var mainFunc = _mainFunc;
	var i = 0;
	var loopFunc = function () {
	  var result = mainFunc(i);
	  if (result === false) {
		// break機能
		return;
	  }
	  i = i + 1;
	  if (i < loopLimit) {
		setTimeout(loopFunc, interval);
	  }
	}
	loopFunc();
}