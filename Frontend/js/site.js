//var backendip = "127.0.0.1:8080/SHP-Web";
var backendip = "192.168.1.1:8080/SHP-Web"
//var backendip = window.location.host  + window.location.pathname;
var baseUrl ="http://" + backendip + "/rest/gui";
var conditionRowId = 0;
var actionRowId = 0;
var newAutomationId = 0;
var jsonThings;
var conditionTypes;
var savedConditions = 0;
var conditionsToSave = 0;
var savedActions = 0;
var actionsToSave = 0;
var deleteActionList = [];
var deleteConditionList = [];
var combo = $("<select></select>");
var actors = $("<select></select>");
var sensors =$("<select></select>");
var automation;

function onMessage(event){
	var jdata = JSON.parse(event.data);
	var element = document.getElementById('thing' + jdata.id);

	element.innerHTML  = jdata.value + ' ' + jdata.unit;
	element.style.background = "red";

	setTimeout(function() {
		element.style.background = "none";
	}, 2000);

	if(jdata.type == "DigitalActor") {
		if(jdata.value == '1'){
			document.getElementById('btn' + jdata.id).value  = "Ausschalten";
			document.getElementById('btn' + jdata.id).setAttribute( "onClick", "javascript: digitalAction(" + jdata.id + ", 0);" );

		} else {
			document.getElementById('btn' + jdata.id).value  = "Einschalten";	
			document.getElementById('btn' + jdata.id).setAttribute( "onClick", "javascript: digitalAction(" + jdata.id + ", 1);" );
		}

	} else if(jdata.type == "AnalogActor"){
		document.getElementById('analog' + jdata.id).value  = jdata.value;
	}
}

function connectWS(){
	ws = new WebSocket("ws://" + backendip + "/websocket");	
	ws.onmessage = onMessage;
}

function getThings(){
	$.getJSON(baseUrl + "/thing", function(thingsData) {
		jsonThings = thingsData;	
		$.getJSON(baseUrl + "/automation/conditionTypes", function(typeData) {
			conditionTypes = typeData;
			fillTables();
		});		
		
	});
}

function digitalAction(id, action) {
	$.post(baseUrl + "/thing", {id:id, value:action}, function(data) {
		if (data.status === "successful") {
			/* yay */
		}
	});
}

function analogAction(id) {
	var value = document.getElementById("analog"+id).value;//$('#'+id).val();
	$.post(baseUrl + "/thing", {id:id, value:value}, function(data) {
		if (data.status === "successful") {
			/* yay */
		}
	});
}

function getAutomationsAndLinkWithTable() {
	$.getJSON(baseUrl + "/automation", function(data) {
		console.log(data);
		automation = data;

		// Remove all options before appending the new options
		$('#comboAuto').find('option').remove();

		$.each(data, function(ai, auto) {			
			  
			 $('#comboAuto').append($("<option></option>")
							.attr("id","autoid" + auto.id)
							.text(auto.name)); 	
		});

		fillCondtionAndAction();
		$('#comboAuto').change(fillCondtionAndAction);
	});
}

function addConditionRow() {
	conditionRowId += 1;

	var row = '<tr class="tbConditionRow" id="conditionRowId' + conditionRowId + '"><td><select>' + sensors.html() + '</select></td><td><select>';
	row += combo.html() + '</select></td>';
	row += '<td><input type="text"></td><td><input type="button" class="button" value="Entfernen" onclick="removeConditionRow(';
	row += conditionRowId + ')"></td></tr>';

	$('#tbCondition tr:last').prev().after(row);
}

function addActionRow() {
	actionRowId += 1;

	var row = '<tr class="tbActionRow" id="actionRowId' + actionRowId + '"><td><select>' + actors.html() + '</select></td>';
	row += '<td><input type="text"></td>';
	row += '<td><input type="button" class="button" value="Entfernen" onclick="removeActionRow(' + actionRowId + ')"></td></tr>';

	$('#tbAction tr:last').prev().after(row);
}

function removeConditionRow(rowId) {
	if ($('#conditionRowId' + rowId).find('td:nth-child(1)').attr('id') && $('#conditionRowId' + rowId).find('td:nth-child(1)').attr('id').startsWith('conditionNameTD')) {
		var conditionIdString = $('#conditionRowId' + rowId).find('td:nth-child(1)').attr('id');
		var conditionId = conditionIdString.replace('conditionNameTD', '');
		var condition = {};

		var selectedIndex = document.getElementById("comboAuto").selectedIndex;
		var options = document.getElementById("comboAuto").options;
		var selectedAutomation = getAutomationById(options[selectedIndex].id.replace('autoid', ''));

		for (var i = 0; i < selectedAutomation.conditions.length; i++) {
			if (selectedAutomation.conditions[i].id == conditionId) {
				condition = selectedAutomation.conditions[i];
				break;
			}
		}

		deleteConditionList.push(condition);
	}

	$('#conditionRowId' + rowId).remove();
}

function removeActionRow(rowId) {
	if ($('#actionRowId' + rowId).find('td:nth-child(1)').attr('id') && $('#actionRowId' + rowId).find('td:nth-child(1)').attr('id').startsWith('actionNameTD')) {
		var actionIdString = $('#actionRowId' + rowId).find('td:nth-child(1)').attr('id');
		var actionId = actionIdString.replace('actionNameTD', '');
		var action = {};

		var selectedIndex = document.getElementById("comboAuto").selectedIndex;
		var options = document.getElementById("comboAuto").options;
		var selectedAutomation = getAutomationById(options[selectedIndex].id.replace('autoid', ''));

		for (var i = 0; i < selectedAutomation.actions.length; i++) {
			if (selectedAutomation.actions[i].id == actionId) {
				action = selectedAutomation.actions[i];
				break;
			}
		}

		deleteActionList.push(action);
	}

	$('#actionRowId' + rowId).remove();
}

function fillCondtionAndAction(){
	var x = document.getElementById("comboAuto").selectedIndex;
	var y = document.getElementById("comboAuto").options;
	var selectedAuto;
	var selectedid =  y[x].id;

	if (selectedid) {
		$.each(automation, function(ai, auto) {
			if(("autoid" + auto.id) === selectedid){
				selectedAuto = auto;		
			}
		});

	} else {
		for (var i = 0; i < automation.length; i++) {
			if (automation[i].id === y[x].value) {
				selectedAuto = automation[i];
				break;
			}
		}
	}

	$(".tbConditionRow").remove(); 		
	$(".tbActionRow").remove(); 

	$('#automationNameField').val(selectedAuto.name);
	$('#automationActivated').prop('checked', selectedAuto.active);

	deleteConditionList = [];
	deleteActionList = [];

	$.each(selectedAuto.conditions, function(ci, condi) {
		var conditionid = condi.id;
		$('#tbCondition tr:last').prev().after(
				'<tr class="tbConditionRow" id="conditionRowId' + conditionRowId + '"><td id="conditionNameTD' + conditionid + '"><select id="conditionThing' + conditionid + '">' + sensors.html() + "</select></td>" + 
				"<td><select id='select" + conditionid + "'>" + combo.html() + "</select></td>" + 
				"<td><input id='condiValue" + conditionid + "' type='text' value='" +condi.value + "'/></td>" +
				'<td><input type="button" class="button" value="Entfernen" onclick="removeConditionRow(' + conditionRowId + ')"></td></tr>' );

		$("#select" + conditionid  + " :nth-child(" +  (condi.type +1) + ")").prop('selected', true);				
		$("#conditionThing" + conditionid + " option[value=" + condi.thing  +"]").prop('selected', true);

		conditionRowId += 1;
	});

	
	$.each(selectedAuto.actions, function(ai, action) {
		var actionid = action.id;
		$('#tbAction tr:last').prev().after(
				'<tr class="tbActionRow" id="actionRowId' + actionRowId + '"><td id="actionNameTD' + actionid + '"><select id="actionThing' + actionid +
				'">' + actors.html() + "</select></td>" + 
				"<td><input id='actionValue" + actionid + "' type='text' value='" +action.value + "'/></td>" +
				'<td><input type="button" class="button" value="Entfernen" onclick="removeActionRow(' + actionRowId + ')"></td></tr>' );

		$("#actionThing" + actionid + " option[value=" + action.thing  +"]").prop('selected', true);

		actionRowId += 1;
	});

}

function fillTables() {
	
	$.each(conditionTypes, function (i, item) {
		combo.append('<option value="' + item.id + '">' + item.info + "</option>");
	});
	
	$.each(jsonThings, function(i, item) {
		if (item.type.endsWith("Actor")) {
			actors.append('<option value="' + item.id + '">' + item.name + '</option>');
		}
	});
	
	$.each(jsonThings, function(i, item) {

		switch(item.type) {
			case "Sensor":
				var unit = (item.unit === undefined) ? "" : item.unit;
				$('#sensorTable tr:last').after('<tr onclick="openPopup(' + item.id + ', \'' + item.name + '\')"><td>' + item.name + '</td><td>' + '<p id=\'thing' + item.id + '\'>' + item.currentValue.value + ' ' + unit + '</p></td></tr>');

				sensors.append('<option value="' + item.id + '">' + item.name + '</option>');
				break;
				
			case "DigitalActor":
				var buttonText = "Ausschalten";
				var value = 0;

				if (item.currentValue.value == 0) {
					value = 1;
					buttonText = "Einschalten";
				}

				$('#actorTable tr:last').after('<tr><td onclick="openPopup(' + item.id + ', \'' + item.name + '\')">' + item.name + '</td><td>' + '<p id=\'thing' + item.id + '\'>' + item.currentValue.value +'</p></td><td><input type="button" class="button" value="' + buttonText + '" onclick="digitalAction(' + item.id + ', ' + value + ')" id="btn'+ item.id +'"></td></tr>');
				break;

			case "AnalogActor":
				$('#actorTable tr:last').after('<tr><td onclick="openPopup(' + item.id + ', \'' + item.name + '\')">' + item.name + '</td><td>' + '<p id=\'thing' + item.id + '\'>' + item.currentValue.value +'</p></td><td><input type="range" min="0" max="100" id="analog' + item.id + '" value="' + item.currentValue.value + '"><input type="button"  class="button" value="Absenden" onclick="analogAction(' + item.id + ')"></td></tr>');
				break;
		}
	});

	getAutomationsAndLinkWithTable();
}

function getAutomationById(id) {
	for (var i = 0; i < automation.length; i++) {
		var temp = automation[i].id;
		if (automation[i].id == id) {
			return automation[i];
		}
	}
}

function submitChanges() {
	var selectedIndex =  document.getElementById('comboAuto').selectedIndex;
	var options = document.getElementById('comboAuto').options;
	var isNewAutomation = options[selectedIndex].value.startsWith("newAuto");


	var automationObj = {actions: [], active: $('#automationActivated').prop('checked'), conditions: [], name: $('#automationNameField').val()};
	options[selectedIndex].innerHTML = automationObj.name;

	if (!isNewAutomation) {
		var id = options[selectedIndex].id;
		automationObj.id = getAutomationById(id.replace('autoid', '')).id;
	}

	for (var i = 0; i < deleteConditionList.length; i++) {
		$.ajax({url: baseUrl + "/automation/condition/" + deleteConditionList[i].id, type: 'DELETE', success: function(result) {
			console.log(result);
		}});
	}
	deleteConditionList = [];

	for (var i = 0; i < deleteActionList.length; i++) {
		$.ajax({url: baseUrl + "/automation/action/" + deleteActionList[i].id, type: 'DELETE', success: function(result) {
			console.log(result);
		}});
	}
	deleteActionList = [];


	$('.tbConditionRow').each(function(i, row) {
		var thing = $(row).find('td:nth-child(1) select').val();
		var type = $(row).find('td:nth-child(2) select').val();
		var value = $(row).find('td:nth-child(3) input').val();

		var newCond = {thing: thing, type: type, value: value};

		if ($(row).find('td:nth-child(1)').attr('id') && $(row).find('td:nth-child(1)').attr('id').startsWith('conditionNameTD')) {
			var idString = $(row).find('td:nth-child(1)').attr('id');
			newCond.id = idString.replace('conditionNameTD', '');
		}

		automationObj.conditions.push(newCond);
	});


	$('.tbActionRow').each(function(i, row) {
		var thing = $(row).find('td:nth-child(1) select').val();
		var value = $(row).find('td:nth-child(2) input').val();

		var newAction = {thing: thing, value: value};

		if ($(row).find('td:nth-child(1)').attr('id') && $(row).find('td:nth-child(1)').attr('id').startsWith('actionNameTD')) {
			var idString = $(row).find('td:nth-child(1)').attr('id');
			newAction.id = idString.replace('actionNameTD', '');
		}

		automationObj.actions.push(newAction);
	});


	saveAutomation(automationObj);
}

function saveAutomation(automationObj) {
	var param = {name:automationObj.name};
	var actionUrl = "/create";

	if ('id' in automationObj) {
		actionUrl = "";
		param.automationid = automationObj.id;
		param.active = automationObj.active;
	}

	$.post(baseUrl + "/automation" + actionUrl, param, function(answer) {
		var newAutoId;

		if ('id' in answer) {
			newAutoId = answer.id;

		} else {
			newAutoId = automationObj.id;
		}

		actionsToSave = automationObj.actions.length;
		for (var i = 0; i < actionsToSave; i++) {
			var action = automationObj.actions[i];
			var actionIdUrl = "";

			param = {name:automationObj.name, value:action.value};


			if ('id' in action) {
				actionIdUrl = "/" + action.id;
				param.thingid = action.thing;

			} else {
				param.autoid = newAutoId;
				param.thing = action.thing;
			}


			$.post(baseUrl + "/automation/action" + actionIdUrl, param, function(manswer) {
				savedActions += 1;
				reloadAutomations();
			});
		}

		conditionsToSave = automationObj.conditions.length;
		for (var i = 0; i < conditionsToSave; i++) {
			var condition = automationObj.conditions[i];
			var conditionIdUrl = "";

			param = {thingid: condition.thing, value: condition.value};


			if ('id' in condition) {
				conditionIdUrl = "/" + condition.id;
				param.conditionType = condition.type;

			} else {
				param.autoid = newAutoId;
				param.type = condition.type;
			}


			$.post(baseUrl + "/automation/condition" + conditionIdUrl, param, function(manswer) {
				savedConditions += 1;
				reloadAutomations();
			});
		}
	});
}

function reloadAutomations() {
	if (savedConditions > 0 && savedConditions == conditionsToSave && savedActions > 0 && savedActions == actionsToSave) {
		savedConditions = 0;
		savedActions = 0;

		getAutomationsAndLinkWithTable();
	}
}

function addAutomation() {
	$(".tbConditionRow").remove(); 		
	$(".tbActionRow").remove(); 

	$('#automationNameField').val("Neue Regel");
	$('#automationActivated').prop('checked', true);

	$('#comboAuto').append(new Option("Neue Regel", "newAuto" + newAutomationId, false, true));

	automation.push({actions: [], active: false, conditions: [], id: "newAuto" + newAutomationId, name: "Neue Regel"});

	newAutomationId += 1;
}

function deleteAutomation(){
	var selectedIndex = document.getElementById('comboAuto').selectedIndex;
	var options = document.getElementById('comboAuto').options;
	var selectedAuto;
	var selectedid =  options[selectedIndex].id;

	if(options[selectedIndex].value.startsWith("newAuto")) {

		// Selected automation was not saved -> Remove locally only
		
		// Remove object from list
		for (var i = 0; i < automation.length; i++) {
			if (automation[i].id === options[selectedIndex].value) {
				automation.splice(i, 1);
				break;
			}
		}

		$('#comboAuto option:selected').remove();

	} else {

		$.each(automation, function(ai, auto) {
			if(("autoid" + auto.id) === selectedid){
				selectedAuto = auto;	
				selectedid = auto.id;
			}
		});

		$.post(baseUrl + "/automation/delete", {automationid:selectedid}, function(data) {
			for (var i = 0; i < automation.length; i++) {
				if (automation[i].id === options[selectedIndex].value) {
					automation.splice(i, 1);
					break;
				}
			}

			console.log('Deleted automation with ID ' + selectedid);
			$('#comboAuto option:selected').remove();
		});
	}

	fillCondtionAndAction();
}

function getAndDisplayDataInChart(thingId, name) {
	$.getJSON(baseUrl + '/thing/' + thingId + '/7', function(result) {
		console.log(result);

		var data = [['Zeit', 'Wert']];

		$.each(result, function(i, item) {
			data.push([new Date(item.time * 1000.0), parseFloat(item.value)]);
		});

		drawChart(data, name);
	});

}

function submit() {
	var email = $('#email').val();
	var password = $('#password').val();
	$.getJSON(baseUrl + "/login/" + email + "/" + password, function(data) {
		if (data.status === "successful") {
			$('#loginForm').hide();
			$('#content').show();
			getThings();
			connectWS();
		}
	});
}