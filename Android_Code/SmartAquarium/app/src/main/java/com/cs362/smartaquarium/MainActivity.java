package com.cs362.smartaquarium;

import static android.content.ContentValues.TAG;

import androidx.appcompat.app.AppCompatActivity;

import android.app.TimePickerDialog;
import android.graphics.PorterDuff;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.PatternMatcher;
import android.provider.ContactsContract;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.animation.AlphaAnimation;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;
import android.widget.ToggleButton;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Locale;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MainActivity extends AppCompatActivity {




    final static int UPDATE = 1;
    final static int ERROR = 2;
    //final static int POST_TO_TERMINAL = 3; //v.1.1


    //flag to prevent light button update loop
    boolean lightBtnChangeBySystem = false;


    //GUI
    ToggleButton lightBtn ;

    ToggleButton feedBtn;
    TextView temperatureTv;

    TextView rtcTimeTV;
    TextView autoFeedTv;

    Button changeAutoFeedBtn;

    Button updateBtn;



    int hr_RTC, min_RTC;

    int hr_Alarm, min_Alarm;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);








        //UI

        lightBtn = findViewById(R.id.lightBtn);
        feedBtn = findViewById(R.id.foodBtn);
        changeAutoFeedBtn = findViewById(R.id.editAutoFeedBtn);

        temperatureTv = findViewById(R.id.temperatureTxt);

        rtcTimeTV = findViewById(R.id.rtcTimeTxt);
        autoFeedTv = findViewById(R.id.autoFeedTxt);
        updateBtn = findViewById(R.id.updateBtn);



        //terminal = findViewById(R.id.terminal);

        setButtonListeners();
        initUiHandler();


        // Tạo Handler để lên lịch gửi thời gian lên Firebase mỗi giây
        Handler handler = new Handler(Looper.getMainLooper());
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                // Lấy thời gian hiện tại và định dạng
                Calendar calendar = Calendar.getInstance();
                SimpleDateFormat sdf = new SimpleDateFormat("HH:mm", Locale.getDefault());
                String currentTime = sdf.format(calendar.getTime());
                currentTime=currentTime.replace(":", ",");

                // Gửi giá trị thời gian lên Firebase
                FirebaseDatabase database = FirebaseDatabase.getInstance();
                DatabaseReference myRef = database.getReference("CRTC");
                myRef.setValue("CRTC:" + currentTime);

                // Lặp lại việc gửi giá trị sau mỗi giây
                handler.postDelayed(this, 100); // 1000ms = 1 giây
            }
        };

        // Bắt đầu lịch trình gửi giá trị lên Firebase
        handler.post(runnable);



    }


    private void setButtonListeners()
    {

        //Listeners


        lightBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if(lightBtn.isChecked())
                {
                    lightBtn.setBackground(getDrawable(R.drawable.bulb_selected));
                    lightBtn.setForeground(getDrawable(R.drawable.bulb_selected));
                    // Write a message to the database
                    FirebaseDatabase database = FirebaseDatabase.getInstance();

                    DatabaseReference myRef = database.getReference("Led");

                    myRef.setValue("L:ON");

                }
                else
                {
                    lightBtn.setBackground(getDrawable(R.drawable.bulb_unselected));
                    lightBtn.setForeground(getDrawable(R.drawable.bulb_unselected));
                    // Write a message to the database
                    FirebaseDatabase database = FirebaseDatabase.getInstance();

                    DatabaseReference myRef = database.getReference("Led");

                    myRef.setValue("L:OFF");
                }





            }


        });



        /*
        feedBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if(!isIpValid()) return;


                //StatusObject sObj = new StatusObject(lightBtn.isSelected());
                Message msg = new Message();
                msg.what = httpHandler.START_STEPPER;
                //msg.obj = sObj;


                t.myHandler.sendMessage(msg);

            }
        });

         */

        /*
        updateBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if(!isIpValid()) return;


                //StatusObject sObj = new StatusObject(lightBtn.isSelected());
                Message msg = new Message();
                msg.what = httpHandler.GET_UPDATE;
                //msg.obj = sObj;


                t.myHandler.sendMessage(msg);

            }
        });

         */



        changeAutoFeedBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                TimePickerDialog tpd = new TimePickerDialog(MainActivity.this,
                        new TimePickerDialog.OnTimeSetListener() {
                            @Override

                            public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
                                String formattedHour = String.format("%02d", hourOfDay);
                                String formattedMinute = String.format("%02d", minute);

                                // Sử dụng formattedHour và formattedMinute trong chuỗi bạn muốn hiển thị
                                String formattedTime = formattedHour + ":" + formattedMinute;
                                Toast.makeText(MainActivity.this, "Alarm Time = " + formattedTime, Toast.LENGTH_SHORT).show();

                                // Định dạng chuỗi để gán vào DatabaseReference
                                String firebaseValue = "CA:" + formattedHour + "," + formattedMinute;

                                // Cập nhật hr_Alarm và min_Alarm với giá trị mới
                                hr_Alarm = hourOfDay;
                                min_Alarm = minute;

                                // Write a message to the database
                                FirebaseDatabase database = FirebaseDatabase.getInstance();
                                DatabaseReference myRef = database.getReference("CA");
                                myRef.setValue(firebaseValue);
                            }

                        }, 12,0,true);


                tpd.updateTime(hr_Alarm,min_Alarm);
                tpd.show();

            }
        });



        updateBtn.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN: {
                        Button view = (Button) v;

                        view.setBackground(getDrawable(R.drawable.connect_unselected));
                        view.setForeground(getDrawable(R.drawable.connect_unselected));
                        break;
                    }
                    case MotionEvent.ACTION_UP:{
                        Button view = (Button) v;
                        view.setBackground(getDrawable(R.drawable.connect_selected));
                        view.setForeground(getDrawable(R.drawable.connect_selected));




                        break;
                    }


                }
                return true;
            }
        });

        feedBtn.setOnClickListener(new View.OnClickListener()  {
            @Override
            public void onClick(View v)  {
                if(feedBtn.isChecked())
                {

                    feedBtn.setBackground(getDrawable(R.drawable.fish_unselected));
                    feedBtn.setForeground(getDrawable(R.drawable.fish_unselected));
                    // Write a message to the database
                    FirebaseDatabase database = FirebaseDatabase.getInstance();

                    DatabaseReference myRef = database.getReference("Servo");

                    myRef.setValue("");

                }
                else {

                    feedBtn.setBackground(getDrawable(R.drawable.fish_selected));
                    feedBtn.setForeground(getDrawable(R.drawable.fish_selected));

                    // Write a message to the database
                    FirebaseDatabase database = FirebaseDatabase.getInstance();

                    DatabaseReference myRef = database.getReference("Servo");

                    myRef.setValue("S:START");



                }


            }


        });



/*
        lightBtn.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked)
                {
                    buttonView.setBackground(getDrawable(R.drawable.bulb_selected));
                    buttonView.setForeground(getDrawable(R.drawable.bulb_selected));

                    if(!lightBtnChangeBySystem)
                    {
                        if(!isIpValid()) return;

                        StatusObject sObj = new StatusObject(isChecked);
                        //terminal.setText(Boolean.toString(btxt.equals("ON")));
                        Message msg = new Message();
                        msg.what = httpHandler.TOGGLE_LIGHT;
                        msg.obj = sObj;

                        t.myHandler.sendMessage(msg);

                    }


                }
                else
                {
                    buttonView.setBackground(getDrawable(R.drawable.bulb_unselected));
                    buttonView.setForeground(getDrawable(R.drawable.bulb_unselected));

                    if(!lightBtnChangeBySystem)
                    {
                        if(!isIpValid()) return;

                        StatusObject sObj = new StatusObject(isChecked);
                        //terminal.setText(Boolean.toString(btxt.equals("ON")));
                        Message msg = new Message();
                        msg.what = httpHandler.TOGGLE_LIGHT;
                        msg.obj = sObj;

                        t.myHandler.sendMessage(msg);

                    }

                }



                lightBtnChangeBySystem = false;
            }
        }); */
    }

    private void initUiHandler()
    {
            DatabaseReference myRef1 = FirebaseDatabase.getInstance().getReference("T");
            DatabaseReference myRef2 = FirebaseDatabase.getInstance().getReference("CA");
            DatabaseReference myRef3 = FirebaseDatabase.getInstance().getReference("CRTC");

            myRef1.addValueEventListener(new ValueEventListener() {
                @Override
                public void onDataChange(DataSnapshot dataSnapshot) {
                    String value = dataSnapshot.getValue(String.class);
                    // Tách phần con sau dấu hai chấm
                    String[] parts = value.split(":");
                    String result = parts[1];
                    temperatureTv.setText(result+"°C");
                }

                @Override
                public void onCancelled(DatabaseError databaseError) {
                    Log.w(TAG, "Failed to read value.", databaseError.toException());
                }
            });

            myRef2.addValueEventListener(new ValueEventListener() {
                @Override
                public void onDataChange(DataSnapshot dataSnapshot) {
                    String value = dataSnapshot.getValue(String.class);
                    // Tìm vị trí của dấu hai chấm
                    int colonIndex = value.indexOf(':');

                    // Lấy các kí tự còn lại sau dấu hai chấm
                    String remainingChars = value.substring(colonIndex + 1);
                    remainingChars=remainingChars.replace(",", ":");
                    autoFeedTv.setText(remainingChars);
                }

                @Override
                public void onCancelled(DatabaseError databaseError) {
                    Log.w(TAG, "Failed to read value.", databaseError.toException());
                }
            });
            myRef3.addValueEventListener(new ValueEventListener() {
                @Override
                public void onDataChange(DataSnapshot dataSnapshot) {
                    String value = dataSnapshot.getValue(String.class);
                    // Tìm vị trí của dấu hai chấm
                    int colonIndex = value.indexOf(':');

                    // Lấy các kí tự còn lại sau dấu hai chấm
                    String remainingChars = value.substring(colonIndex + 1);
                    remainingChars=remainingChars.replace(",", ":");
                    rtcTimeTV.setText(remainingChars);
                }

                @Override
                public void onCancelled(DatabaseError databaseError) {
                    Log.w(TAG, "Failed to read value.", databaseError.toException());
                }
            });

    }

};