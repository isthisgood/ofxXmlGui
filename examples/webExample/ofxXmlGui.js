
gui = {};
function createGui(parent, file) {
    $.ajax({
        type: "GET",
        url: "gui.xml",
        dataType: "xml",
        success: function(data) {
            gui = createGuiFromXml(parent, data);
            console.log(gui);
         }
    });
}

function createGuiFromXml(parent, xml) {
    
    //console.log(xml.childNodes);
    // loop through nodes - look in chrome console
    // should do this with livecoding
    // ignore "text"
    // create an object consisting of a "type" which is the tag name and its attributes
    // and look into children
    var g = {};
    for(i = 0; i < xml.childNodes.length; i++) {
        var el = xml.childNodes[i];
        g[el.name] = {};

        if(el.childNodes.length>0) {
            var children = createGuiFromXml();
        }
    }
    return g;
}