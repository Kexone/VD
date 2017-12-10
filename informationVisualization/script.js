d3.csv("test.csv", function(error,values) {

	  // Various formatters.
	  var formatNumber = d3.format(",d"),
	  formatChange = d3.format("+,d"),
	  formatDate = d3.time.format("%d-%b");

	  var nestByDate = d3.nest()
	  .key(function(d) { return d3.time.day(d.date); });



	  if (error) {
	  	return console.warn(error);
	  }
	  values.forEach(function(d, i) {
	  	d.id = i;
	  	d.date = parseDate(d.date);
	  	d.name = d.name;
	  	d.quantity = d.quantity;
	  	d.kcal = d.kcal;
	  	d.protein = d.protein;
	  	d.carbohydrates = d.carbohydrates;
	  	d.sugar = d.sugar;
	  	d.fat = d.fat;
	  });

	  var allValues = crossfilter(values),
	  all = allValues.groupAll(),
	  date = allValues.dimension(function(d) { return d.date; }),
	  dates = date.group(d3.time.day),
	  kcal = allValues.dimension(function(d) { return d.kcal; }),
	  kcalls = kcal.group(Math.floor),
	  fat = allValues.dimension(function(d) { return d.fat}),
	  fats = fat.group(Math.floor),
	  protein = allValues.dimension(function(d) { return d.protein }),
	  proteins = protein.group(Math.floor);


	  console.log(values);


	  var charts = [

	  barChart()
	  .dimension(kcal)
	  .group(kcalls)
	  .x(d3.scale.linear()
	  	.domain([0, 450])
	  	.rangeRound([0, 10 * 21])),

	  barChart()
	  .dimension(protein)
	  .group(proteins)
	  .x(d3.scale.linear()
	  	.domain([0, 100])
	  	.rangeRound([0, 10 * 21])),

	  barChart()
	  .dimension(fat)
	  .group(fats)
	  .x(d3.scale.linear()
	  	.domain([0, 100])
	  	.rangeRound([0, 10 * 21])),

	  barChart()
	  .dimension(date)
	  .group(dates)
	  .round(d3.time.day.round)
	  .x(d3.time.scale()
	  	.domain([new Date(2017, 6, 22), new Date(2017, 10, 22)])
	  	.rangeRound([0, 10 * 90]))
	  .filter([new Date(2017, 6, 22), new Date(2017, 10, 22)])

	  ];


	  var chart = d3.selectAll(".chart")
	  .data(charts)
	  .each(function(chart) { chart.on("brush", drawAll).on("brushend", drawAll); });

	  var list = d3.selectAll(".list")
	  .data([foodList]);
	//print total of values
	d3.selectAll("#total")
	.text(formatNumber(allValues.size()));

	drawAll();

	function draw(method) {
		d3.select(this).call(method);
	}
	function drawAll() {
		chart.each(draw);
		list.each(draw);
		d3.select("#active").text(formatNumber(all.value()));
	}


	function parseDate(d) {
		return new Date(2017,
			d.substring(0, 2) - 1,
			d.substring(2, 4));
	}


	window.reset = function(i) {
		charts[i].filter(null);
		drawAll();
	};

	function foodList(div) {
		var foodbyDate = nestByDate.entries(date.top(100));

		div.each(function() {

			var date = d3.select(this).selectAll(".date")
			.data(foodbyDate, function(d) { return d.key; });

			date.enter().append("div")
			.attr("class", "date")
			.append("div")
			.text(function(d) { return formatDate(d.values[0].date); })


			.attr("class", "headers")
			.text(function(d) { return "Name " + " Quantity "+ " Kcal "+ " Protein "+ " Carbohydrates "+ " Sugar "+ " Fat " + formatDate(d.values[0].date) })
			date.exit().remove();

			var foods = date.order().selectAll(".food")
			.data(function(d) { return d.values; }, function(d) { return d.index; });

			var foodEntry = foods.enter().append("div")
			.attr("class", "food");

			foodEntry.append("div")
			.attr("class", "name")
			.text(function(d) { return d.name; });

			foodEntry.append("div")
			.attr("class", "quantity")
			.text(function(d) { return d.quantity; });

			foodEntry.append("div")
			.attr("class", "kcal")
			.text(function(d) { return d.kcal; });

			foodEntry.append("div")
			.attr("class", "protein")
			.text(function(d) { return d.protein; });

			foodEntry.append("div")
			.attr("class", "carbohydrates")
			.text(function(d) { return d.carbohydrates; });

			foodEntry.append("div")
			.attr("class", "sugar")
			.text(function(d) { return d.sugar; });

			foodEntry.append("div")
			.attr("class", "fat")
			.text(function(d) { return d.fat; });

			foods.exit().remove();

			foods.order();
		});
	}


	function barChart() {
		if (!barChart.id) barChart.id = 0;

		var margin = {top: 10, right: 10, bottom: 20, left: 10},
		x,
		y = d3.scale.linear().range([100, 0]),
		id = barChart.id++,
		axis = d3.svg.axis().orient("bottom"),
		brush = d3.svg.brush(),
		brushDirty,
		dimension,
		group,
		round;

		function chart(div) {
			var width = x.range()[1],
			height = y.range()[0];

			y.domain([0, group.top(1)[0].value]);

			div.each(function() {
				var div = d3.select(this),
				g = div.select("g");

        // Create the skeletal chart.
        if (g.empty()) {
        	div.select(".title").append("a")
        	.attr("href", "javascript:reset(" + id + ")")
        	.attr("class", "reset")
        	.text("reset")
        	.style("display", "none");

        	g = div.append("svg")
        	.attr("width", width + margin.left + margin.right)
        	.attr("height", height + margin.top + margin.bottom)
        	.append("g")
        	.attr("transform", "translate(" + margin.left + "," + margin.top + ")");

        	g.append("clipPath")
        	.attr("id", "clip-" + id)
        	.append("rect")
        	.attr("width", width)
        	.attr("height", height);

        	g.selectAll(".bar")
        	.data(["background", "foreground"])
        	.enter().append("path")
        	.attr("class", function(d) { return d + " bar"; })
        	.datum(group.all());

        	g.selectAll(".foreground.bar")
        	.attr("clip-path", "url(#clip-" + id + ")");

        	g.append("g")
        	.attr("class", "axis")
        	.attr("transform", "translate(0," + height + ")")
        	.call(axis);

          // Initialize the brush component with pretty resize handles.
          var gBrush = g.append("g").attr("class", "brush").call(brush);
          gBrush.selectAll("rect").attr("height", height);
          gBrush.selectAll(".resize").append("path").attr("d", resizePath);
      }

        // Only redraw the brush if set externally.
        if (brushDirty) {
        	brushDirty = false;
        	g.selectAll(".brush").call(brush);
        	div.select(".title a").style("display", brush.empty() ? "none" : null);
        	if (brush.empty()) {
        		g.selectAll("#clip-" + id + " rect")
        		.attr("x", 0)
        		.attr("width", width);
        	} else {
        		var extent = brush.extent();
        		g.selectAll("#clip-" + id + " rect")
        		.attr("x", x(extent[0]))
        		.attr("width", x(extent[1]) - x(extent[0]));
        	}
        }

        g.selectAll(".bar").attr("d", barPath);
    });

			function barPath(groups) {
				var path = [],
				i = -1,
				n = groups.length,
				d;
				while (++i < n) {
					d = groups[i];
					path.push("M", x(d.key), ",", height, "V", y(d.value), "h9V", height);
				}
				return path.join("");
			}

			function resizePath(d) {
				var e = +(d == "e"),
				x = e ? 1 : -1,
				y = height / 3;
				return "M" + (.5 * x) + "," + y
				+ "A6,6 0 0 " + e + " " + (6.5 * x) + "," + (y + 6)
				+ "V" + (2 * y - 6)
				+ "A6,6 0 0 " + e + " " + (.5 * x) + "," + (2 * y)
				+ "Z"
				+ "M" + (2.5 * x) + "," + (y + 8)
				+ "V" + (2 * y - 8)
				+ "M" + (4.5 * x) + "," + (y + 8)
				+ "V" + (2 * y - 8);
			}
		}

		brush.on("brushstart.chart", function() {
			var div = d3.select(this.parentNode.parentNode.parentNode);
			div.select(".title a").style("display", null);
		});

		brush.on("brush.chart", function() {
			var g = d3.select(this.parentNode),
			extent = brush.extent();
			if (round) g.select(".brush")
				.call(brush.extent(extent = extent.map(round)))
			.selectAll(".resize")
			.style("display", null);
			g.select("#clip-" + id + " rect")
			.attr("x", x(extent[0]))
			.attr("width", x(extent[1]) - x(extent[0]));
			dimension.filterRange(extent);
		});

		brush.on("brushend.chart", function() {
			if (brush.empty()) {
				var div = d3.select(this.parentNode.parentNode.parentNode);
				div.select(".title a").style("display", "none");
				div.select("#clip-" + id + " rect").attr("x", null).attr("width", "100%");
				dimension.filterAll();
			}
		});

		chart.margin = function(_) {
			if (!arguments.length) return margin;
			margin = _;
			return chart;
		};

		chart.x = function(_) {
			if (!arguments.length) return x;
			x = _;
			axis.scale(x);
			brush.x(x);
			return chart;
		};

		chart.y = function(_) {
			if (!arguments.length) return y;
			y = _;
			return chart;
		};

		chart.dimension = function(_) {
			if (!arguments.length) return dimension;
			dimension = _;
			return chart;
		};

		chart.filter = function(_) {
			if (_) {
				brush.extent(_);
				dimension.filterRange(_);
			} else {
				brush.clear();
				dimension.filterAll();
			}
			brushDirty = true;
			return chart;
		};

		chart.group = function(_) {
			if (!arguments.length) return group;
			group = _;
			return chart;
		};

		chart.round = function(_) {
			if (!arguments.length) return round;
			round = _;
			return chart;
		};

		return d3.rebind(chart, brush, "on");
	}





});

