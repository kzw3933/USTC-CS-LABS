let body = document.querySelector("body")
let header = document.querySelector("header")
let menuCtrl = document.querySelector("#menu-ctrl")
let menuCtrlIcon = document.querySelector("#menu-ctrl i")
let menuLeft = true

menuCtrl.onclick = ()=>{
    if(menuLeft){
        menuCtrlIcon.className = "fa fa-arrow-circle-right fa-3x"
        menuCtrl.className = "right"
        header.className = "right"
        body.className = "right"
        menuLeft = false
    }
    else {
        menuCtrlIcon.className = "fa fa-arrow-circle-left fa-3x"
        menuCtrl.className = "left"
        header.className = "left"
        body.className = "left"
        menuLeft = true
    }
}
