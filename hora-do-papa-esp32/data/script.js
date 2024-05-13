const url = `http://${location.host}`;

showExistentActivations();

const formEl = document.querySelector('form');

formEl.addEventListener('submit', event => {
  event.preventDefault();

  const formData = new FormData(event.target);

  const data = Object.fromEntries(formData.entries());
  var repeat = formData.getAll('repeat');
  data.repeat = txArray(repeat);

  addActivation(data);
});

function txArray(repeat) {
  var repeatArray = [];
  repeat.includes("sun") ? repeatArray.push(true) : repeatArray.push(false);
  repeat.includes("mon") ? repeatArray.push(true) : repeatArray.push(false);
  repeat.includes("tue") ? repeatArray.push(true) : repeatArray.push(false);
  repeat.includes("wed") ? repeatArray.push(true) : repeatArray.push(false);
  repeat.includes("thu") ? repeatArray.push(true) : repeatArray.push(false);
  repeat.includes("fri") ? repeatArray.push(true) : repeatArray.push(false);
  repeat.includes("sat") ? repeatArray.push(true) : repeatArray.push(false);
  return repeatArray;
}

async function showExistentActivations() {
  var db;
  await fetch(`${url}/activations`, {
    method: 'GET',
    headers: {
      'Accept': 'application/json'
    }
  }).then(res => {
      if (!res.ok) {
        throw new Error('Network response was not ok');
      }
      return res.json();
    })
    .then(json => db = json)
    .catch(error => console.error('/activations fetch error:', error));

  console.log(db);

  db.activations.forEach(activation => createActCard(activation));
}

function createActCard(activation) {
  const div = document.createElement('div');
  div.className = "box";

  const media = document.createElement('article');
  media.className = "media";

  const mediaLeft = document.createElement('div');
  mediaLeft.className = "media-left";
  
  const time = document.createElement('h3');
  time.className = "title is-3";
  time.textContent = activation.time;
  mediaLeft.appendChild(time);
  
  const mediaContent = document.createElement('div');
  mediaContent.className = "media-content is-align-self-center";
  const tags = document.createElement('div');
  tags.className = "tags is-justify-content-center";
  
  activation.repeat.forEach((day, index) => {
    const tag = document.createElement('span');
    tag.textContent = getDay(index);
    day ? tag.className = "tag is-success" : tag.className = "tag";
    tags.appendChild(tag);
  });
  mediaContent.appendChild(tags);

  const mediaRight = document.createElement('div');
  mediaRight.className = "media-right";
  
  const button = document.createElement('button');
  button.className = "button is-danger is-outlined";
  button.innerHTML = '<span class="icon is-small"><i class="fas fa-trash-alt"></i></span>';
  button.addEventListener('click', () => deleteActivation(activation, div));
  mediaRight.appendChild(button);
  
  media.appendChild(mediaLeft);
  media.appendChild(mediaContent);
  media.appendChild(mediaRight);
  
  div.appendChild(media);

  const results = document.querySelector('.results');
  results.appendChild(div);
}

function deleteActivation(activation, div) {
  fetch(`${url}/delete`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(activation)
  }).then(res => {
      if (!res.ok) {
        throw new Error('Network response was not ok');
      }
      return res.json();
    })
    .then(json => {
      console.log(json);
      // Delete activation card
      div.parentNode.removeChild(div);
    })
    .catch(error => console.error('/delete fetch error:', error));
}

function getDay(index) {
  const days = ["D", "S", "T", "Q", "Q", "S", "S"];
  return days[index];
}

function addActivation(data) {
  fetch(`${url}/add`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(data)
  }).then(res => {
      if (!res.ok) {
        throw new Error('Network response was not ok');
      }
      return res.json();
    })
    .then(json => {
      console.log(json);
      createActCard(data);
      
    })
    .then(formEl.reset())
    .catch(error => console.error('/add fetch error:', error));
}