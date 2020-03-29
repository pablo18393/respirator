

byte SerialX_available() {  
    return(Serial1.available());  
}

char SerialX_peek() {  
    return(Serial1.peek());  
}


char SerialX_read() { 
    return(Serial1.read());  
}

void SerialX_print(int payload) {    
    Serial1.print(payload);}


void SerialX_print(long payload) {  
    Serial1.print(payload); 
}



void SerialX_print(char payload) {  
    Serial1.write(payload);  
}

void SerialX_print(char* payload) {  
    Serial1.write(payload);  
}
