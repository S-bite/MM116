
function createElementWithInnerText(tag, text) {
    var e = document.createElement(tag);
    e.innerText = text;
    return e;
}

function constructTable(data) {
    var table = document.createElement("table");
    table.setAttribute("class", "table");

    var thead = document.createElement("thead");
    var tr = document.createElement("tr");
    tr.appendChild(createElementWithInnerText("th", "id"))
    tr.appendChild(createElementWithInnerText("th", "score"))
    for (var i = 1; i <= 30; i++) {
        tr.appendChild(createElementWithInnerText("th", "Seed" + i))
    }
    thead.appendChild(tr);
    table.appendChild(thead);
    var tbody = document.createElement("tbody");
    var NumOfSubmissions = data["nextid"] - 1;
    for (var i = 0; i <= NumOfSubmissions; i++) {
        var tr = document.createElement("tr")
        var submission = data["submissions"][i];
        tr.appendChild(createElementWithInnerText("td", i));
        tr.appendChild(createElementWithInnerText("td", 0));
        var score = 0;
        for (var j = 1; j <= 30; j++) {
            console.log(submission["seed" + j]["score"])
            tr.appendChild(createElementWithInnerText("td", submission["seed" + j]["score"]));
            score += submission["seed" + j]["score"];
        }
        tr.childNodes[1].innerText = score;
        tbody.appendChild(tr);
    }
    table.appendChild(tbody);
    document.getElementById("container").appendChild(table)
    console.log(data["nextid"])
}

fetch('./submissions.json')
    .then(res => res.json())
    .then(data => {
        constructTable(data)
    });