# New York City Subway Display
This repository contains code for a household display of NYC subway arrival times (similar to the ones hanging in many stations.) Built on an ESP32,
arduino code makes requests for specific trains at specific stations to an [AWS API Gateway](https://aws.amazon.com/api-gateway/), 
which triggers an [AWS lambda function](https://aws.amazon.com/lambda/). This function requests subway data from the 
[MTA's subway API](https://api.mta.info/#/landing), processes and filters the resulting Protobuf structure, and returns only relevant times for the 
requested train and station. AWS services are used as a proxy between the ESP32 and MTA API to simplify code structure, bypasses the memory limitations
of the ESP32, and allow extensibility for future API calls.
