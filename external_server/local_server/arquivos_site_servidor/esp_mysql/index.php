<!--
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

-->
<?php

$servername = "127.0.0.1";

// REPLACE with your Database name
$dbname = "esp_data";
// REPLACE with Database user
$username = "esp_data";
// REPLACE with Database user password
$password = "eujogodemf";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Conexão falhou: " . $conn->connect_error);
} 

$sql = "SELECT id, value1, value2, value3, value4, value5, value6, reading_time FROM sensor order by reading_time desc limit 40";

$result = $conn->query($sql);

while ($data = $result->fetch_assoc()){
    $sensor_data[] = $data;
}

$readings_time = array_column($sensor_data, 'reading_time');

// ******* Uncomment to convert readings time array to your timezone ********
$i = 0;
/*
foreach ($readings_time as $reading){
    // Uncomment to set timezone to - 1 hour (you can change 1 to any number)
    $readings_time[$i] = date("Y-m-d H:i:s", strtotime("$reading - 3 hours"));
    // Uncomment to set timezone to + 4 hours (you can change 4 to any number)
    //$readings_time[$i] = date("Y-m-d H:i:s", strtotime("$reading + 4 hours"));
    $i += 1;
}
*/

$value1 = json_encode(array_reverse(array_column($sensor_data, 'value1')), JSON_NUMERIC_CHECK);
$value2 = json_encode(array_reverse(array_column($sensor_data, 'value2')), JSON_NUMERIC_CHECK);
$value3 = json_encode(array_reverse(array_column($sensor_data, 'value3')), JSON_NUMERIC_CHECK);
$value4 = json_encode(array_reverse(array_column($sensor_data, 'value4')), JSON_NUMERIC_CHECK);
$value5 = array_reverse(array_column($sensor_data, 'value5'));
$value6 = array_reverse(array_column($sensor_data, 'value6'));
$reading_time = json_encode(array_reverse($readings_time), JSON_NUMERIC_CHECK);

//echo $value1;
//echo $value2;
//echo $value3;
//echo $value4; 
//echo $reading_time;

$result->free();
$conn->close();
?>

<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
  <link rel="icon" type="image/x-icon" href="favico.ico">
  <script type="text/javascript" src="highcharts.js"></script>
  <style>
    body {
      color: white;
      margin: 0;
      background-color: #333333;
    }
    .espestacao {
      font-family: Arial, Helvetica, sans-serif;
      font-size: 1.3rem;
      letter-spacing: 1px;
    }
    #logo {
      height: 50px;
    }
    #cabecalho{
      height: 50px;
      margin-bottom: 80px;
      padding: 10px;
      background-color: #4b4b4b;
    }
    #logo_div{
      display: flex;
      height: 100%;
      width: 15%;
      float: left;
    }
    #text_div{
      height: 100%;
      width: 70%;
      display: flex;
      float: left;
      align-items: center;
      justify-content: center;
    }
    #chart-temperature{
      margin-bottom: 50px;
    }
    #chart-humidity{
      margin-bottom: 50px;
    }
    #chart-chuva{
      margin-bottom:50px;
    }
    #chart-pressure{
      margin-bottom:50px;
    }
    #chart-altitude{
      margin-bottom:50px;
    }
  </style>
</head>
<body>
  <div id="cabecalho">
    <div id="logo_div">
      <img id="logo" src="logo.png">
    </div>
    <div id="text_div">
      <p class="espestacao">Estação Metereológica</p>
    </div>
  </div>

  <div id="container"></div>
  chuva: 
  <?php echo end($value5); ?>
  vento:
  <?php echo end($value6); ?>
  <div id="chart-temperature" class="container"></div>
  <div id="chart-humidity" class="container"></div>
  <div id="chart-pressure" class="container"></div>
</body>

<script>

var value1 = <?php echo $value1; ?>;
var value2 = <?php echo $value2; ?>;
var value3 = <?php echo $value3; ?>;
var value4 = <?php echo $value4; ?>;
var reading_time = <?php echo $reading_time; ?>;

Highcharts.setOptions({
    colors: ['#058DC7', '#50B432', '#ED561B', '#DDDF00', '#24CBE5', '#64E572', '#FF9655', '#FFF263', '#6AF9C4'],
    chart: {
        backgroundColor: '#333333',
        borderWidth: 0,
        plotBackgroundColor: '#333333',
        plotBorderWidth: 0
    },
    title: {
        style: {
            color: '#e6e6e6',
            font: '16px "Trebuchet MS", Verdana, sans-serif'
        }
    },
    subtitle: {
        style: {
            color: '#e6e6e6',
            font: '12px "Trebuchet MS", Verdana, sans-serif'
        }
    },
    xAxis: {
        gridLineWidth: 1,
        gridLineColor: '#666666',
        lineColor: '#595959',
        tickColor: '#595959',
        labels: {
            style: {
                color: '#e6e6e6',
                font: '11px Trebuchet MS, Verdana, sans-serif'
            }
        },
        title: {
            style: {
                color: '#e6e6e6',
                fontWeight: 'bold',
                fontSize: '12px',
                fontFamily: 'Trebuchet MS, Verdana, sans-serif'

            }
        }
    },
    yAxis: {
        alternateGridColor: null,
        gridLineColor: '#666666',
        minorTickInterval: 'none',
        lineColor: '#595959',
        lineWidth: 1,
        tickWidth: 1,
        labels: {
            style: {
                color: '#e6e6e6',
                font: '11px Trebuchet MS, Verdana, sans-serif'
            }
        },
        title: {
            style: {
                color: '#e6e6e6',
                fontWeight: 'bold',
                fontSize: '12px',
                fontFamily: 'Trebuchet MS, Verdana, sans-serif'
            }
        }
    },
    legend: {
        itemStyle: {
            font: '9pt Trebuchet MS, Verdana, sans-serif',
            color: 'white'

        },
        itemHoverStyle: {
            color: '#e6e6e6'
        },
        itemHiddenStyle: {
            color: 'gray'
        }
    },
    credits: {
        style: {
            right: '10px'
        }
    },
    labels: {
        style: {
            color: '#e6e6e6'
        }
    }
});


/*
// create the chart
Highcharts.chart('container', {

xAxis: {
    type: 'datetime'
},

series: [{
    data: [29.9, 71.5, 106.4, 129.2, 144.0, 176.0, 135.6, 148.5, 216.4, 194.1, 95.6, 54.4],
    pointStart: Date.UTC(2010, 0, 1),
    pointInterval: 3600 * 1000 // one hour
}]
});
*/


var chartTH = new Highcharts.Chart({
  chart:{ renderTo:'chart-temperature' }, 
  title: { text: 'Temperatura x Umidade' },
  series: [{
    showInLegend: false,
    data: value2,
  },{
  showInLegend: false,
  data: value1
  }
  ],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true, color: '#e6e6e6'  },
    },
    series: {color: '#42f584' }
  },
  xAxis: {
    type: 'time',
    dateTimeLabelFormats: { second: '%H:%M' },
    categories: reading_time
  },
  yAxis: {
    title: { text: 'Umidade (%)' }
  },
  credits: { enabled: false }
});

var chartH = new Highcharts.Chart({
  chart:{ renderTo:'chart-humidity' }, 
  title: { text: 'Umidade' },
  series: [{
    showInLegend: false,
    data: value2
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true, color: '#e6e6e6' },
    },
    series: {color: '#42f584' }
  },
  xAxis: {
    type: 'time',
    dateTimeLabelFormats: { second: '%H:%M:%S' },
    categories: reading_time
  },
  yAxis: {
    title: { text: 'Umidade (%)' }
  },
  credits: { enabled: false }
});

var chartA = new Highcharts.Chart({
  chart:{ renderTo:'chart-pressure' },
  title: { text: 'Pressão' },
  series: [{
    showInLegend: false,
    data: value3
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true, color: '#e6e6e6' },
    },
    series: { color: '#b35be3' }
  },
  xAxis: {
    type: 'datetime',
    //dateTimeLabelFormats: { second: '%H:%M:%S' }
    categories: reading_time
  },
  yAxis: {
    title: { text: 'Pressão (Pa)' }
  },
  credits: { enabled: false }
});


</script>
</html> 
