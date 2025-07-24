# ThingsBoard Rule Chain: Low RSSI Alarm with Mattermost Webhook

This guide details the final, working steps to create a ThingsBoard rule chain that triggers a critical alarm when a device's RSSI (signal strength) is low, sends a formatted notification to Mattermost, and automatically clears the alarm when the signal is restored.

## Final Result

This is the final, correctly formatted notification you will receive in Mattermost:

![Final Mattermost Notification](images/mattermost_notification.png)

## 1. Create the Rule Chain

1.  Navigate to **Rule Chains** in the ThingsBoard UI.
2.  Click the `+` button to create a new rule chain.
3.  Name it `RSSI Alarm Chain` and click **Add**.

## 2. Configure the Rule Chain Nodes

Drag the nodes from the palette onto the canvas and connect them as shown in the diagram below.

![Final Rule Chain Layout](images/rule_chain_layout.png)

### Node 1: `script` (Filter)

This node checks if the RSSI value is below the threshold.

-   **Name**: `Check rssi value`
-   **Type**: `script` filter
-   **JavaScript Code**:
    ```javascript
    // Triggers the 'True' path if the device's RSSI is less than -70
    return msg.rssi < -70;
    ```
-   **Connection**: Connect the `Input` node to this node with the `Success` relation.

### Node 2: `create alarm`

This node generates the alarm and saves the current RSSI value in the alarm's details using a JavaScript function.

-   **Name**: `Low Signal Strength`
-   **Type**: `create alarm`
-   **Alarm type**: `Low Signal Strength`
-   **Alarm severity**: `Critical`
-   **Alarm Details (Dynamic value)**:
    ```javascript
    var details = {
        "rssi": msg.rssi
    };
    
    return details;
    ```
-   **Connection**: Connect the `Check rssi value` node to this node with the `True` relation.

### Node 3: `script` (Transformation)

This node reads the alarm data, formats a timestamp for UTC+3, and builds the final JSON payload for Mattermost.

-   **Name**: `Format Mattermost Message`
-   **Type**: `script` transformation
-   **JavaScript Code**:
    ```javascript
    // Create a JavaScript Date object from the alarm's timestamp (in milliseconds).
    var alarmDate = new Date(msg.createdTime);
    
    // Format the date and time to be readable for the 'Europe/Istanbul' timezone (UTC+3).
    var formattedTime = alarmDate.toLocaleString('en-GB', {
        timeZone: 'Europe/Istanbul',
        day: '2-digit',
        month: 'short',
        year: 'numeric',
        hour: '2-digit',
        minute: '2-digit',
        second: '2-digit',
        hour12: false
    });
    
    // The RSSI value is stored inside the 'details' object of the alarm.
    var rssiValue = msg.details.rssi;
    
    var newMsg = {
      "text": "### 🚨 Critical Alarm: Low Signal Strength\n" +
              "*   **Device**: " + msg.originatorName + "\n" +
              "*   **Time**: " + formattedTime + " (UTC+3)\n" +
              "*   **RSSI Value**: " + rssiValue + "\n" +
              "*   **Alarm Type**: " + msg.type + "\n" +
              "*   **Severity**: " + msg.severity
    };
    
    return {msg: newMsg, metadata: metadata, msgType: msg.msgType};
    ```
-   **Connection**: Connect the `create alarm` node to this node with the `Created` relation.

### Node 4: `rest api call`

This node sends the final, formatted message to your Mattermost webhook URL.

-   **Name**: `Send to Mattermost`
-   **Type**: `rest api call`
-   **HTTP Method**: `POST`
-   **Endpoint URL**: Your Mattermost Webhook URL
-   **Headers**:
    -   **Header**: `Content-Type`
    -   **Value**: `application/json`
-   **Connection**: Connect the `Format Mattermost Message` node to this node with the `Success` relation.

### Node 5: `clear alarm`

This node clears the alarm once the signal strength returns to normal, making the alarm repeatable.

-   **Name**: `Clear Low Signal Alarm`
-   **Type**: `clear alarm`
-   **Alarm type**: `Low Signal Strength` (Must be the *exact same name* as in the `create alarm` node).
-   **Connection**: Connect the `Check rssi value` node to this node with the `False` relation.

## 3. Set as Root Rule Chain

1.  Click the **Set root** button in the rule chain editor's title bar.
2.  Alternatively, go to **Device Profiles** > **(your device profile)** > **Rule chain** tab and select `RSSI Alarm Chain`.

