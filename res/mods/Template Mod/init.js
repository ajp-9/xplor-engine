function init()
{
	var name = "Template"
	var author = "Me";
	var version = 1.0;
	var description = "A template mod for people who want to create mods.";
	
	var entities = ['template.entity'];
	
	var items = ['template.item'];
	
	return [name, author, version, description, entities, items];
}

initReturn = init();
console.log(initReturn[0]);