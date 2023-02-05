const ACTION_PRESS = 0,
    ACTION_DOWN = 1,
    ACTION_UP = 2;


var socket = io();

var initialTouch, currentTouch, priorTouch, touchDistance,
    verbose_input_events = false;


function relMotionAccelProfile(deltaX, deltaY){

    let delta = Math.hypot(deltaX, deltaY), // delta magnitude
        scale = (delta + 1)**1.4;  // profile equation

    return {x: scale*deltaX/delta, y:scale*deltaY/delta};
}

function touchMoveHandler(event){

    let primaryTouch = event;

    priorTouch = currentTouch;
    currentTouch = primaryTouch;
    
    touchDistance = Math.hypot(
        currentTouch.touches[0].clientX - initialTouch.touches[0].clientX, 
        currentTouch.touches[0].clientX - initialTouch.touches[0].clientX
        );

    let deltaX = currentTouch.touches[0].clientX - priorTouch.touches[0].clientX,
        deltaY = currentTouch.touches[0].clientY - priorTouch.touches[0].clientY;

    if(verbose_input_events)
        console.log("touchMoveHandler:", deltaX, deltaY);
    
    socket.emit('relMotion',
            relMotionAccelProfile(deltaX, deltaY)
        );
}

function touchStartHandler(event){

    initialTouch = currentTouch = event;
    touchDistance = 0;

}


function touchEndHandler(event){

    let touchDuration =  event.timeStamp - initialTouch.timeStamp;

    if(verbose_input_events)
        console.log("touchEndHandler:", touchDuration, touchDistance);

    // left click
    if(touchDuration < 200 && touchDistance < 50){
        socket.emit('buttonPress',{
            button: 1,
            action: ACTION_PRESS
        });
    }
    
    // right click
    if(touchDuration > 1000 && touchDistance < 50){
        socket.emit('buttonPress',{
            button: 3,
            action, ACTION_PRESS
        });
    }

}

function initMobileKeyPressElem(element){
    element.value = ".."; // extra chars used to allow BackSpace, Delete events
    element.setSelectionRange(1, 1);
}

function mobileKeyPressHandler(event){

    switch(event.inputType) {

        case "insertCompositionText":
        case "insertText":
            socket.emit("keyPress",{
                key: event.data,
                action: ACTION_PRESS
                });
            break;

        case "deleteContentBackward":
            socket.emit("keyPress",{
                key: "BackSpace",
                action: ACTION_PRESS
                });
            break;

        case "deleteContentForward":
            socket.emit("keyPress",{
                key: "Delete",
                action: ACTION_PRESS
                });  
                break;

        case "insertLineBreak":
            socket.emit("keyPress",{
                key: "Enter",
                action: ACTION_PRESS
                });  
                break;

        default:
            console.log(`Unknown inputType ${event.inputType}`, event)
        } 

        initMobileKeyPressElem(event.target);

}

