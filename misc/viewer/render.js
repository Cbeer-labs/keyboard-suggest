let stage = acgraph.create('container');
let bounds = stage.getBounds();
const color_map = {
    'p': 'red ',
    'f': 'green ',
    'r': 'blue ',
    'z': 'yellow ',
    'a': 'magenta ',
};
const color_coef_map = {
    'p': 1000,
    'f': 10,
    'r': 100,
    'z': 100,
    'a': 100,
};
function color(text) {
    const k = text[0];
    const c = Math.max(Math.abs(parseFloat(text.substr(1)))/color_coef_map[k], 0.3);
    return color_map[k] + c
}

function paint(data) {

    let rows = data.split('\n');

    stage.removeChildren();
    for (let i = 0; i < rows.length; ++i) {
        let items = rows[i].split('\t');
        for (let j = 0; j < items.length; ++j) {
            const rect_size = Math.min(bounds.width / items.length, bounds.height / rows.length);
            let text = items[j];
            if (text.includes("Epoch")) {
                stage.addChild(acgraph.text(j * rect_size, i * rect_size, text).fontSize('14pt'));
                continue;
            }
            if (text.length === 0) {
                continue;
            }
            stage.rect(j * rect_size, i * rect_size, rect_size, rect_size).fill(color(text));
            // stage.addChild(acgraph.text(j * rect_size, i * rect_size + rect_size / 2, text).fontSize('6pt'));
        }
    }
}

function myLoop() {
    setTimeout(function () {
        fetch('/data').then(function (response) {
            if (!response.ok) {
                // console.log(response);
                throw new Error(`HTTP error! status: ${response.status}`);
            }
            return response.text();
        }).then(data => {
            paint(data);
            // console.log(data);
        });
        myLoop();
    }, 100);
}

myLoop();
