class EventTimer{
  private:
    unsigned long endTime;
  public:
    //EventTimer(){}
    void Start(unsigned long ms){
      endTime = millis()+ms;
    }
    bool isExpired(){
      return(millis() > endTime);
    }
    void Cancel(){
      endTime = millis();
    }
};
