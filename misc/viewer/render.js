function onInputChanged(e) {
    const text = document.getElementById("input_text").value;

    fetch('/data?input=' + text).then(function (response) {
        if (!response.ok) {
            // console.log(response);
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        return response.text();
    }).then(data => {
        const obj = JSON.parse(data);
        let html = '';
        for (let s of obj) {
            html = html + '\n' +
                `<input type="button" id="suggestion" onclick="onButtonClick(event)" value="${s}"/>`;
        }
        let container = document.getElementById("suggestions");
        container.innerHTML = html;
    });
}

function onButtonClick(event) {
    // if (event == null)
    //     return;
    let text = document.getElementById("input_text").value;
    let words = text.split(' ');
    words[words.length - 1] = event.target.value;
    document.getElementById("input_text").value = words.join(' ') + ' ';
    onInputChanged(null);
}

onInputChanged(null);