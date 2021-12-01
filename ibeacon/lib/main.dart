import 'dart:async';
import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_beacon/flutter_beacon.dart';

main() async {
  // runApp(const MyApp());
}

Future<void> initBeacon() async {
  try {
    await flutterBeacon.initializeAndCheckScanning;
  } on PlatformException catch (e) {}
}

void monitorBeacons(List<Region> monitor) {
  if (Platform.isIOS) {
    // iOS platform, at least set identifier and proximityUUID for region scanning
    monitor.add(Region(
        identifier: 'Apple Airlocate',
        proximityUUID: 'E2C56DB5-DFFB-48D2-B060-D0F5A71096E0'));
  } else {
    // Android platform, it can ranging out of beacon that filter all of Proximity UUID
    monitor.add(Region(identifier: 'com.beacon'));
  }

  // to start monitoring beacons
  StreamSubscription _streamMonitoring =
      flutterBeacon.monitoring(monitor).listen((MonitoringResult result) {
    // result contains a region, event type and event state
  });

  // to stop monitoring beacons
  _streamMonitoring.cancel();
}

List<Beacon> rangeBeacons() {

  List<Beacon> beacons = [];
  List<Region> range = [];

  if (Platform.isIOS) {
    range.add(Region(
        identifier: 'Apple AirLocate',
        proximityUUID: "C6C4C829-4FD9-4762-837C-DA24C665015A"));
  } else {
    range.add(Region(identifier: 'com.beacon'));
  }
  StreamSubscription _streamRanging =
      flutterBeacon.ranging(range).listen((RangingResult result) {
        beacons.addAll(result.beacons);
      });

  _streamRanging.cancel();
  return beacons;
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {


    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        // This is the theme of your application.
        //
        // Try running your application with "flutter run". You'll see the
        // application has a blue toolbar. Then, without quitting the app, try
        // changing the primarySwatch below to Colors.green and then invoke
        // "hot reload" (press "r" in the console where you ran "flutter run",
        // or simply save your changes to "hot reload" in a Flutter IDE).
        // Notice that the counter didn't reset back to zero; the application
        // is not restarted.
        primarySwatch: Colors.blue,
      ),
      home: const MyHomePage(),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({Key? key}) : super(key: key);

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  @override
  Widget build(BuildContext context) {
    initBeacon();
    List<Beacon> beacons = rangeBeacons()
    // This method is rerun every time setState is called, for instance as done
    // by the _incrementCounter method above.
    //
    // The Flutter framework has been optimized to make rerunning build methods
    // fast, so that you can just rebuild anything that needs updating rather
    // than having to individually change instances of widgets.
    return Scaffold(
      appBar: AppBar(
          // Here we take the value from the MyHomePage object that was created by
          // the App.build method, and use it to set our appbar title.
          ),
      body: Center(
        child: Container(
            child: Padding(
          padding: const EdgeInsets.only(
            left: 20,
            right: 20,
          ),
          child: Column(
              mainAxisAlignment: MainAxisAlignment.start,
              crossAxisAlignment: CrossAxisAlignment.stretch,
              children: [_rangeListView(beacons)]),
        )),
      ), // This trailing comma makes auto-formatting nicer for build methods.
    );
  }

  _rangeListView(List<Beacon> beacons) {
    return Container(
        height: 100,
        decoration: const BoxDecoration(
          border: Border(
            top: BorderSide(color: Colors.white),
            bottom: BorderSide(color: Colors.white),
          ),
        ),
        child: ListView.builder(
            scrollDirection: Axis.horizontal,
            itemCount: beacons.length,
            itemBuilder: (context, index) {
              return Container(
                  width: 50,
                  child: Card(child: Center(child: Text('${beacons[index]}'))));
            }));
  }
}
