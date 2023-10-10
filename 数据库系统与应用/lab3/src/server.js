import express from 'express';
import bodyParser from 'body-parser';
import cors from 'cors';

import paperAPIs from './api/paper';
import teacherAPIs from './api/teacher';
import projectAPIs from './api/project';
import lessonAPIs from './api/lesson';

const app = express();
app.use(cors());
app.use(bodyParser.json());
app.use(express.static('static'));
paperAPIs(app);
teacherAPIs(app);
projectAPIs(app);
lessonAPIs(app);
app.listen(3000);